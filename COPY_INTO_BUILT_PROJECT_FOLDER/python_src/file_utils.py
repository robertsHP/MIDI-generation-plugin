import pickle
import torch
import json
import os

import ai_utils

def save_model_and_tokenizer(model, model_config, tokenizer, directory, folder_name, sequence_length, cancel_event):
    folder_path = os.path.join(directory, folder_name)

    if cancel_event.is_set(): return
    
    os.makedirs(folder_path, exist_ok=True)

    if cancel_event.is_set(): return

    model_path = os.path.join(folder_path, "model.pth")
    tokenizer_path = os.path.join(folder_path, "tokenizer.pkl")
    config_path = os.path.join(folder_path, "model.config.json")

    if cancel_event.is_set(): return

    torch.save(model.state_dict(), model_path)
    print(f"Model saved!")

    if cancel_event.is_set(): return

    with open(tokenizer_path, 'wb') as f:
        pickle.dump(tokenizer, f)
    print(f"Tokenizer saved!")

    if cancel_event.is_set(): return

    model_config['sequence_length'] = sequence_length
    with open(config_path, 'w') as f:
        json.dump(model_config, f)
    print(f"Configuration saved!")

def load_model_and_tokenizer(directory, folder_name, device, cancel_event):
    folder_path = os.path.join(directory, folder_name)

    if cancel_event.is_set(): return

    model_path = os.path.join(folder_path, "model.pth")
    tokenizer_path = os.path.join(folder_path, "tokenizer.pkl")
    config_path = os.path.join(folder_path, "model.config.json")

    if cancel_event.is_set(): return

    with open(config_path, 'r') as f:
        model_config = json.load(f)

    if cancel_event.is_set(): return

    sequence_length = model_config.pop('sequence_length', None)

    if cancel_event.is_set(): return

    model = ai_utils.MidiTransformer(device=device, **model_config)

    if cancel_event.is_set(): return

    model.load_state_dict(torch.load(model_path, map_location=device))
    model.eval()

    if cancel_event.is_set(): return

    with open(tokenizer_path, 'rb') as f:
        tokenizer = pickle.load(f)

    return model, tokenizer, sequence_length