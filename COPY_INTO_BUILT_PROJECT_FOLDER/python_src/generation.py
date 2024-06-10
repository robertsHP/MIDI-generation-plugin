import torch
import pretty_midi

import ai_utils
import ai_functions
import midi_utils
import file_utils

import random

def generate (**kwargs) :
    torch.seed()

    FEATURES = ['pitch', 'duration', 'step', 'velocity']
    MODEL_DIR = str(kwargs['model_path'])
    FOLDER_NAME = str(kwargs['selected_folder_name'])
    INPUT_FILE_PATH = kwargs['temp_midi_file_path']
    INPUT_FILE = pretty_midi.PrettyMIDI(INPUT_FILE_PATH)
    # DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    DEVICE = torch.device("cpu")

    print("Loading model and tokenizer...")

    if kwargs['cancel_event'].is_set(): return

    model, tokenizer, sequence_length = file_utils.load_model_and_tokenizer(MODEL_DIR, FOLDER_NAME, DEVICE)

    print("Preparing input data...")

    if kwargs['cancel_event'].is_set(): return

    midi_data, max_seq_length = midi_utils.get_midi_data(INPUT_FILE, sequence_length, None)

    print("Pre-processing input data...")

    if kwargs['cancel_event'].is_set(): return

    midi_data = ai_functions.clean_dataframes(midi_data)
    midi_data = ai_functions.normalize_dataframes(midi_data)
    midi_data = ai_functions.quantize_dataframes(midi_data)
    midi_data = ai_functions.trim_columns_for_dataframes(midi_data, FEATURES)

    print("Pre-processing tokenizing input data...")

    if kwargs['cancel_event'].is_set(): return

    dataset = ai_utils.MIDIDataset(
        midi_data,
        max_seq_length,
        FEATURES
    )

    dataset = tokenizer.tokenize(dataset)

    # print(dataset.data)

    print("Getting input data mask...")

    if kwargs['cancel_event'].is_set(): return

    input_seq = dataset.get_data().to(DEVICE)
    # src_mask = model.generate_mask(input_seq.size(0)).to(DEVICE)

    print("Passing data into model...")

    if kwargs['cancel_event'].is_set(): return

    model.eval()
    generated_sequence = input_seq

    with torch.no_grad():
        for _ in range(6):
            src_mask = model.generate_mask(len(generated_sequence)).to(DEVICE)

            temperature = random.uniform(0.3, 1.5)

            output = model(generated_sequence, src_mask)

            if output.size(0) == 0:
                continue

            output = output[-1] / temperature
            
            next_sequence = torch.multinomial(torch.softmax(output, dim=-1), num_samples=1).mT

            generated_sequence = torch.cat((generated_sequence, next_sequence), dim=0)

    print("Result successfully generated!")
    print("Converting result into a suitable format...")

    if kwargs['cancel_event'].is_set(): return

    df = tokenizer.detokenize(generated_sequence)
    df = ai_functions.denormalize(df)

    print("Result converted...")

    if kwargs['cancel_event'].is_set(): return

    pm = midi_utils.convert_data_into_pretty_midi(df)
    pm.write(INPUT_FILE_PATH)

    print("MIDI notes generated!")
    print()