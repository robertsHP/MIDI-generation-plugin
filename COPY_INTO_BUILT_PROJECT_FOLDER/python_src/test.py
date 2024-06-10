import torch
# print(torch.__version__)


# python_version = sys.version_info
# print(f"Python version: {python_version.major}.{python_version.minor}.{python_version.micro}")

cuda_version = torch.version.cuda
print(f"CUDA version: {cuda_version}")

# result = "cuda" if torch.cuda.is_available() else "cpu"
# device = torch.device(result)

# print(result)
# print(f"Current device: {device}")



# import pandas as pd
# import numpy as np
# import torch

# # Special tokens
# PAD_TOKEN = "<pad>"
# START_TOKEN = "<s>"
# END_TOKEN = "</s>"

# # Assign integer IDs to special tokens
# special_tokens = {PAD_TOKEN: 0, START_TOKEN: 1, END_TOKEN: 2}
# next_token_id = 3

# # Example DataFrame
# data = {
#     'Feature1': [0.1, 0.5, 0.9, 0.2],
#     'Feature2': [0.3, 0.7, 0.8, 0.4],
#     'Feature3': [0.2, 0.8, 0.7, 0.1]
# }
# df = pd.DataFrame(data)

# print(df)

# # Tokenization
# def tokenize_dataframe(df, special_tokens, next_token_id):
#     token_mapping = special_tokens.copy()
#     tokenized_rows = []

#     for _, row in df.iterrows():
#         tokenized_row = []

#         for value in row:
#             if value not in token_mapping:
#                 token_mapping[value] = next_token_id
#                 next_token_id += 1
#             tokenized_row.append(token_mapping[value])

#         tokenized_rows.append(tokenized_row)

#     return tokenized_rows, token_mapping, next_token_id

# tokenized_rows, token_mapping, next_token_id = tokenize_dataframe(df, special_tokens, next_token_id)

# # Adding start and end tokens to each row
# tokenized_rows = [[special_tokens[START_TOKEN]] + row + [special_tokens[END_TOKEN]] for row in tokenized_rows]

# # Padding sequences
# max_length = max(len(row) for row in tokenized_rows)
# padded_rows = [row + [special_tokens[PAD_TOKEN]] * (max_length - len(row)) for row in tokenized_rows]

# # Convert to tensor
# input_tensor = torch.tensor(padded_rows, dtype=torch.long)

# # Print tokenized DataFrame
# print(f"Token Mapping: {token_mapping}")
# print(f"Input Tensor: {input_tensor}")

# # Reverse mapping for detokenization
# reverse_token_mapping = {v: k for k, v in token_mapping.items()}

# # Detokenization Function
# def detokenize_tensor(tensor, reverse_token_mapping):
#     detokenized_rows = []

#     for row in tensor:
#         detokenized_row = []
#         for token in row:
#             value = reverse_token_mapping[token.item()]
#             if value not in [PAD_TOKEN, START_TOKEN, END_TOKEN]:
#                 detokenized_row.append(value)
#         detokenized_rows.append(detokenized_row)

#     return detokenized_rows

# # Detokenize
# detokenized_rows = detokenize_tensor(input_tensor, reverse_token_mapping)

# # Convert back to DataFrame
# detokenized_df = pd.DataFrame(detokenized_rows, columns=df.columns)

# # Print Detokenized DataFrame
# print(f"Detokenized DataFrame: \n{detokenized_df}")