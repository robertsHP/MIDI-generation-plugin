## MIDI-generation-plugin
This is a free plugin for MIDI note generation which provides a way to train models using your own datasets and later using them for generating MIDI notes. This project does not come with any existing datasets in order to avoid any copyright
issues.

![image](https://github.com/robertsHP/MIDI-generation-plugin/assets/63208048/3b956f27-2416-435e-9408-ea59b53050a5)

## Project Building
In order to build the project it is required to download JUCE from https://juce.com/download/, one of the JUCE supported code editors that are available on the Projucer app, 
pybind11 ([https://github.com/pybind/pybind11?tab=readme-ov-file](https://github.com/pybind/pybind11)) and at least Python 3.9. For Python code to execute the Pytorch, 
pretty_midi and pandas libraries need to be installed with pip (which comes with Python) using a terminal.
Project building instructions:
1. Download or clone this repository.
2. Open "MIDI-generation-plugin.jucer" with Projucer.
3. Add an export target in the "Exporters" section and choose "Release"
4. Choose In "Header Search Paths" from the pybind11 download add the file path to the include folder as well as the includes for the Python version to be used for building. For "Extra Binary Search Paths" add the Python libs folder path.

![image](https://github.com/robertsHP/MIDI-generation-plugin/assets/63208048/941e2ae4-7ceb-4a5e-a022-8989d74f736d)

7. Open the choosen IDE, select plugin extension type and build the project.
8. Copy the built plugin file and the contents of "COPY_INTO_BUILT_PROJECT_FOLDER" in the same plugin directory where the plugin file is located. (models, data and python_src)
