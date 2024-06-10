from typing import List
import pandas as pd

import torch
import torch.nn as nn
from torch.optim import Adam

def clean_dataframes(midi_data: List[pd.DataFrame], cancel_event) -> List[pd.DataFrame]:
    if cancel_event.is_set(): return

    cleaned_data = []
    for df in midi_data:
        if cancel_event.is_set(): return

        # Noņem notis ar neiespējamiem sākuma un beigu laikiem
        df = df[df['start'] < df['end']]

        if cancel_event.is_set(): return

        # Remove notes with pitch/velocity values outside valid MIDI ranges
        df = df[(df['pitch'] >= 0) & (df['pitch'] <= 127)]
        df = df[(df['velocity'] >= 0) & (df['velocity'] <= 127)]

        if cancel_event.is_set(): return

        # Ensure each note starts after the previous one ends
        df = df.sort_values(by='start').reset_index(drop=True)

        if cancel_event.is_set(): return

        cleaned_data.append(df)
    return cleaned_data

def normalize_dataframes(midi_data: List[pd.DataFrame], cancel_event) -> List[pd.DataFrame]:
    if cancel_event.is_set(): return

    for df in midi_data:
        if cancel_event.is_set(): return

        #Normalizācija ir priekš 127, jo "pitch" un "velocity" vērtības ir no 0 līdz 127
        df['pitch'] = df['pitch'] / 127
        df['velocity'] = df['velocity'] / 127
    return midi_data

def denormalize(df: pd.DataFrame, cancel_event) -> pd.DataFrame:
    if cancel_event.is_set(): return

    #127, jo "pitch" un "velocity" vērtības ir no 0 līdz 127
    df['pitch'] = (df['pitch'] * 127).astype(int)  
    df['velocity'] = (df['velocity'] * 127).astype(int)
    return df

def quantize_dataframes(midi_data: List[pd.DataFrame], cancel_event) -> List[pd.DataFrame]:
    note_division = 16 #64

    if cancel_event.is_set(): return

    def quantize_time(t, beat_length=0.5):
        sixteenth_note_length = beat_length / note_division
        return round(t / sixteenth_note_length) * sixteenth_note_length

    if cancel_event.is_set(): return

    beat_length = 0.5

    for df in midi_data:
        if cancel_event.is_set(): return

        if 'start' in df.columns and 'end' in df.columns:
            if cancel_event.is_set(): return

            df['start'] = df['start'].apply(quantize_time)

            if cancel_event.is_set(): return

            df['end'] = df['end'].apply(quantize_time)

            if cancel_event.is_set(): return

            df['prev_start'] = df['prev_start'].apply(quantize_time)

            if cancel_event.is_set(): return

            zero_duration_mask = df['end'] == df['start']

            if cancel_event.is_set(): return

            df.loc[zero_duration_mask, 'end'] += beat_length / note_division 

            if cancel_event.is_set(): return

            df['duration'] = df['end'] - df['start']
            df['step'] = df['start'] - df['prev_start']

    return midi_data

def trim_columns_for_dataframes (data: List[pd.DataFrame], columns: List[str], cancel_event) -> List[pd.DataFrame]:
    if cancel_event.is_set(): return

    return [df.loc[:, columns] for df in data]

def train_and_validate(epochs, model, train_loader, test_loader, cancel_event):
    optimizer = Adam(model.parameters(), lr=5e-5)
    loss_fn = nn.CrossEntropyLoss()

    for epoch in range(epochs):
        model.train()  # Training mode
        train_loss = 0
        for input_seq in train_loader:
            if cancel_event.is_set(): return

            input_seq = input_seq.to(model.device)
            src_mask = model.generate_mask(input_seq.size(0)).to(model.device)

            if cancel_event.is_set(): return

            output = model(input_seq, src_mask)

            if cancel_event.is_set(): return

            loss = loss_fn(output.view(-1, model.n_tokens), input_seq.view(-1))
            optimizer.zero_grad()

            if cancel_event.is_set(): return

            loss.backward()
            optimizer.step()

            if cancel_event.is_set(): return

            train_loss += loss.item()

        if cancel_event.is_set(): return

        avg_train_loss = train_loss / len(train_loader)

        if cancel_event.is_set(): return

        avg_val_loss = evaluate(model, test_loader, model.device, loss_fn, cancel_event)

        if cancel_event.is_set(): return

        print(f'Epoch {epoch+1}: Train Loss: {avg_train_loss}, Val Loss: {avg_val_loss}')

def evaluate(model, dataloader, device, loss_fn, cancel_event):
    model.eval()  # Evaluation mode
    total_loss = 0

    if cancel_event.is_set(): return

    with torch.no_grad():
        for input_seq in dataloader:
            if cancel_event.is_set(): return

            input_seq = input_seq.to(device)
            src_mask = model.generate_mask(input_seq.size(0)).to(device)

            if cancel_event.is_set(): return

            output = model(input_seq, src_mask)

            if cancel_event.is_set(): return

            loss = loss_fn(output.view(-1, model.n_tokens), input_seq.view(-1))
            total_loss += loss.item()

            if cancel_event.is_set(): return

    return total_loss / len(dataloader)