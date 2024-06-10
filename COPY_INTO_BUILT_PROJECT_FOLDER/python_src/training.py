import glob

from torch.utils.data import DataLoader
import torch

import midi_utils
import ai_utils
import ai_functions
import file_utils

def train (**kwargs) :
    torch.seed()

    FEATURES = ['pitch', 'duration', 'step', 'velocity']
    EPOCHS = int(kwargs['training_config']['epochs'])
    BATCH_SIZE = int(kwargs['training_config']['batch_size'])
    SEQUENCE_LENGTH = int(kwargs['training_config']['sequence_length'])
    MODEL_DIR = str(kwargs['model_path'])
    DATA_DIR = str(kwargs['data_path'])
    FOLDER_NAME = str(kwargs['selected_folder_name'])
    DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    model_config = {
        'n_tokens': None,  # Number of unique tokens in your dataset
        'n_inp': int(kwargs['model_config']['input_dim']),  # Input dimension
        'n_head': int(kwargs['model_config']['attention_heads']),  # Number of attention heads
        'n_hid': int(kwargs['model_config']['feed_forward_dim']),  # Dimension of the feedforward network model
        'n_layers':int(kwargs['model_config']['hidden_layers']),  # Number of hidden layers
    }

    filenames = glob.glob(DATA_DIR+'/'+FOLDER_NAME+'/**.mid*', recursive=True)

    if kwargs['cancel_event'].is_set(): return

    print("Reading data...")

    midi_data, max_seq_length = midi_utils.read_all_midi_files(
        filenames, 
        SEQUENCE_LENGTH, 
        None,
        kwargs['cancel_event']
    )

    if kwargs['cancel_event'].is_set(): return

    print("Pre-processing...")

    midi_data = ai_functions.clean_dataframes(midi_data, kwargs['cancel_event'])

    if kwargs['cancel_event'].is_set(): return

    midi_data = ai_functions.normalize_dataframes(midi_data, kwargs['cancel_event'])

    if kwargs['cancel_event'].is_set(): return

    midi_data = ai_functions.quantize_dataframes(midi_data, kwargs['cancel_event'])

    if kwargs['cancel_event'].is_set(): return

    midi_data = ai_functions.trim_columns_for_dataframes(midi_data, FEATURES, kwargs['cancel_event'])

    if kwargs['cancel_event'].is_set(): return

    dataset = ai_utils.MIDIDataset(
        midi_data,
        max_seq_length,
        FEATURES,
    )

    if kwargs['cancel_event'].is_set(): return

    print("Tokenizing...")

    tokenizer = ai_utils.Tokenizer(FEATURES, max_seq_length)

    if kwargs['cancel_event'].is_set(): return

    dataset = tokenizer.tokenize(dataset, kwargs['cancel_event'])

    if kwargs['cancel_event'].is_set(): return

    print("Splitting into training and test sets...")

    train_set, test_set = dataset.get_training_and_test_sets(kwargs['cancel_event'])

    if kwargs['cancel_event'].is_set(): return

    print("Creating data loaders...")

    train_loader = DataLoader(train_set, batch_size=BATCH_SIZE, shuffle=True, pin_memory=True)
    test_loader = DataLoader(test_set, batch_size=BATCH_SIZE, shuffle=True, pin_memory=True)

    model_config['n_tokens'] = len(tokenizer.vocabulary)

    model = ai_utils.MidiTransformer(device=DEVICE, **model_config)

    if kwargs['cancel_event'].is_set(): return

    print("Training...")

    ai_functions.train_and_validate(EPOCHS, model, train_loader, test_loader, kwargs['cancel_event'])

    if kwargs['cancel_event'].is_set(): return

    print("Training complete!")
    print("Saving...")

    if kwargs['cancel_event'].is_set(): return

    file_utils.save_model_and_tokenizer(model, model_config, tokenizer, MODEL_DIR, FOLDER_NAME, SEQUENCE_LENGTH, kwargs['cancel_event'])

    print("Model has been trained and saved!")
    print()