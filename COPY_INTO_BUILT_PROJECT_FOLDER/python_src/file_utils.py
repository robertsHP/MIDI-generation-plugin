import pickle
import torch
import json
import os

import ai_utils

def save_model_and_tokenizer(model, model_config, tokenizer, directory, folder_name, sequence_length):
    folder_path = os.path.join(directory, folder_name)
    
    os.makedirs(folder_path, exist_ok=True)

    model_path = os.path.join(folder_path, "model.pth")
    tokenizer_path = os.path.join(folder_path, "tokenizer.pkl")
    config_path = os.path.join(folder_path, "model.config.json")

    torch.save(model.state_dict(), model_path)
    print(f"Model saved!")

    with open(tokenizer_path, 'wb') as f:
        pickle.dump(tokenizer, f)
    print(f"Tokenizer saved!")

    model_config['sequence_length'] = sequence_length
    with open(config_path, 'w') as f:
        json.dump(model_config, f)
    print(f"Configuration saved!")

def load_model_and_tokenizer(directory, folder_name, device):
    folder_path = os.path.join(directory, folder_name)

    model_path = os.path.join(folder_path, "model.pth")
    tokenizer_path = os.path.join(folder_path, "tokenizer.pkl")
    config_path = os.path.join(folder_path, "model.config.json")

    with open(config_path, 'r') as f:
        model_config = json.load(f)

    sequence_length = model_config.pop('sequence_length', None)

    model = ai_utils.MidiTransformer(device=device, **model_config)
    model.load_state_dict(torch.load(model_path, map_location=device))
    model.eval()

    with open(tokenizer_path, 'rb') as f:
        tokenizer = pickle.load(f)

    return model, tokenizer, sequence_length