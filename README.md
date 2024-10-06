## MIDI-generation-plugin
This is a free plugin for MIDI note generation which provides a way to train models using your own datasets and later using them for generating MIDI notes. This project does not come with any existing datasets in order to avoid any copyright issues.
This project has been made with the following external libraries and tools:
1. JUCE
2. Pytorch
3. pretty_midi
4. Pandas
5. JUCE
6. Pybind11 

![image](https://github.com/robertsHP/MIDI-generation-plugin/assets/63208048/3b956f27-2416-435e-9408-ea59b53050a5)

## How to use?

The plugin consists of two subsections called "Training" and "Generate":
1. The "Training" subsection shows all available datasets from which you can select a specific dataset folder. The dataset folders are located in the "./data" directory where the availability of ".midi" or ".mid" files in each folder is automatically checked and displayed on the interface. When changes are made or new data is added, the list can be refreshed with the "Refresh" button. On the right hand side several fields are displayed indicating the training configurations and model properties of the new model. Pressing the "Train" button runs the Python code to train the new transformer model. The training process can be aborted by pressing the "Cancel" button. Each time the Python code runs the "print" function with the entered text, the output is displayed in the console. If any kind of error occurs in the C++ or Python code, it is output to the console.
2. The "Generation" subsection shows all the trained transformer models available from "./models" directory. The information for each model is stored in separate folders with a pre-assigned name in the training section. If new models are added to the directory, the list can be refreshed with the "Refresh" button. The top left part shows the note input, where a MIDI file is pulled in, which is fed to the selected transformer as input. As the file is pulled in, the colour changes from red to green, allowing the "Generate" button to be pressed to run the Python code to generate the result. The result is available in the top right corner where, just like the input, it changes from red to green, meaning that the generated result can be dragged onto your DAW or desktop. Each time the "print" function in the Python code is run, it appears in the console. If any kind of C++ or Python code error occurs then it is shown on to the console.
