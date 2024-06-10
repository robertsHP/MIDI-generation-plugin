from typing import List
import pandas as pd
from collections import defaultdict

import math

from torch.utils.data import Dataset

import torch
import torch.nn as nn

class MIDIDataset(Dataset):
    def __init__(self, data: List[pd.DataFrame], max_seq_length, columns: List[str]):
        self.data = data
        self.max_seq_length = max_seq_length
        self.columns = columns

    def get_data (self) :
        return torch.tensor(self.data, dtype=torch.long)

    def __getitem__(self, idx):
        return torch.tensor(self.data[idx], dtype=torch.long)
    
    def __len__(self):
        return len(self.data)

    def get_training_and_test_sets (self, cancel_event, training_percentage = 0.8) :
        train_size = int(training_percentage * len(self.data))

        if cancel_event.is_set(): return

        train_set = MIDIDataset(self.data[:train_size], self.max_seq_length, self.columns)

        if cancel_event.is_set(): return

        test_set = MIDIDataset(self.data[train_size:], self.max_seq_length, self.columns)

        return train_set, test_set

class Tokenizer:
    def __init__(self, columns, max_seq_length):
        self.columns = columns
        self.max_seq_length = (max_seq_length * len(columns)) + 2  # Include start and end tokens
        self.special_tokens = {
            '[PAD]': 0,
            '[START]': 1,
            '[END]': 2
        }
        self.next_token_id = 3
        self.vocabulary = defaultdict(self._get_next_token_id)
        self.vocabulary.update(self.special_tokens)

    def _get_next_token_id(self):
        token_id = self.next_token_id
        self.next_token_id += 1
        return token_id

    def tokenize(self, dataset: MIDIDataset, cancel_event) -> MIDIDataset:
        dataset.max_seq_length = self.max_seq_length
        final_data = []

        if cancel_event.is_set(): return

        for df in dataset.data:
            if cancel_event.is_set(): return

            tokenized_data = [self.special_tokens['[START]']]
            for row in df.itertuples(index=False, name=None):
                if cancel_event.is_set(): return

                tokenized_row = [self.vocabulary[value] for value in row]
                tokenized_data.extend(tokenized_row)

                if cancel_event.is_set(): return

            tokenized_data.append(self.special_tokens['[END]'])

            if cancel_event.is_set(): return

            while len(tokenized_data) < self.max_seq_length:
                if cancel_event.is_set(): return

                tokenized_data.append(self.special_tokens['[PAD]'])

            if cancel_event.is_set(): return

            final_data.append(tokenized_data[:self.max_seq_length])

        dataset.data = final_data
        return dataset

    def detokenize(self, predicted_token_ids, cancel_event) -> pd.DataFrame:
        if cancel_event.is_set(): return

        if isinstance(predicted_token_ids, torch.Tensor):
            predicted_token_ids = predicted_token_ids.tolist()

        if cancel_event.is_set(): return

        inv_vocabulary = {v: k for k, v in self.vocabulary.items() if isinstance(k, (int, float, str))}
        inv_vocabulary.update({0: '[PAD]', 1: '[START]', 2: '[END]'})

        if cancel_event.is_set(): return

        detokenized_data = []
        for row in predicted_token_ids:
            if cancel_event.is_set(): return

            token_buffer = []

            for token_id in row:
                if cancel_event.is_set(): return

                value = inv_vocabulary.get(token_id, None)

                if value == '[START]':
                    token_buffer = []
                elif value == '[END]':
                    break
                elif value != '[PAD]' :
                    token_buffer.append(value)

                if cancel_event.is_set(): return

                if len(token_buffer) == len(self.columns):
                    detokenized_data.append(token_buffer)
                    token_buffer = []

        return pd.DataFrame(detokenized_data, columns=self.columns)

class PositionalEncoding(nn.Module):
    def __init__(self, d_model, max_len=5000):
        super(PositionalEncoding, self).__init__()

        dim_indices = torch.arange(0, d_model, 2).float()
        div_term = 10000 ** (dim_indices / d_model)
        position = torch.arange(0, max_len, dtype=torch.float).unsqueeze(1)
        pe = torch.zeros(max_len, d_model)
        pe[:, 0::2] = torch.sin(position * div_term)
        pe[:, 1::2] = torch.cos(position * div_term)
        pe = pe.unsqueeze(1)

        self.register_buffer('pe', pe)

    def forward(self, x):
        x = x + self.pe[:x.size(0), :]
        return x

class MidiTransformer(nn.Module):
    def __init__(self, device, n_tokens, n_inp, n_head, n_hid, n_layers, dropout=0.2):
        super(MidiTransformer, self).__init__()

        self.device = device
        self.n_tokens = n_tokens
        self.n_inp = n_inp
        self.n_head = n_head
        self.n_hid = n_hid
        self.n_layers = n_layers
        
        self.embedding = nn.Embedding(n_tokens, n_inp)
        self.pos_encoder = PositionalEncoding(n_inp)
        self.dropout = nn.Dropout(dropout)
        self.decoder_layer = nn.TransformerDecoderLayer(n_inp, n_head, n_hid, dropout=dropout)
        self.transformer_decoder = nn.TransformerDecoder(self.decoder_layer, num_layers=n_layers)
        self.output_layer = nn.Linear(n_inp, n_tokens)

        self.to(device)

    def generate_mask(self, size):
        mask = torch.zeros(size, size)
        mask = torch.tril(torch.ones(size, size))
        mask = mask.masked_fill(mask == 0, float('-inf'))
        mask = mask.masked_fill(mask == 1, float(0.0))
        return mask

    def forward(self, src, src_mask):
        src = torch.clamp(src, 0, self.n_tokens - 1)

        src = self.embedding(src)
        src = self.pos_encoder(src)
        src = self.dropout(src)
        output = self.transformer_decoder(src, src, src_mask)
        output = self.output_layer(output)
        return output