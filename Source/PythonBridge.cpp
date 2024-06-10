/*
  ==============================================================================

    PythonBridge.cpp
    Created: 12 May 2024 10:44:41am
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PythonBridge.h"

#include "PluginProcessor.h"

//==============================================================================

PythonBridge::PythonBridge() : threadRunning(true), taskRunning(false), cancel_event(py::none())
{
}
PythonBridge::~PythonBridge ()
{
    kill();
}

void PythonBridge::kill() {
    {
        std::lock_guard<std::mutex> lock(mutex);
        threadRunning = false;

        condition.notify_one();
    }
    if (workerThread) {
        cancelCurrentOperation();

        if (workerThread->joinable()) {
            workerThread->join();
        }
    }
}

void PythonBridge::init() {
    if (!Py_IsInitialized()) {
        py::initialize_interpreter();
    };
}

void PythonBridge::finalize() {
    if (Py_IsInitialized()) {
        py::finalize_interpreter();
    }
}

void PythonBridge::workerThreadFunction() {
    init();
    {
        while (threadRunning) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mutex);
                condition.wait(lock, [this] { return !tasks.empty() || !threadRunning; });
                if (!threadRunning && tasks.empty()) {
                    break;
                }
                if (!tasks.empty()) {
                    task = std::move(tasks.front());
                    tasks.pop();
                }
            }
            if (task) {
                task();
            }
        }
    }
    finalize();
}

void PythonBridge::enqueueTask(const std::function<void()>& task) {
    {
        std::lock_guard<std::mutex> lock(mutex);
        tasks.push(task);
    }
    condition.notify_one();
}

void PythonBridge::callPythonFunction(std::string moduleName, std::string funcName, std::map<std::string, std::any> inputArgs, ConsoleComponent& consoleComponent) {
    try {
        consoleComponent.appendText("Starting function...");

        py::gil_scoped_acquire acquire;

        consoleComponent.appendText("Preparing function...");

        redirectStdout(consoleComponent);
        goToPythonPath();

        py::dict pyInputArgs;

        if (cancel_event.is_none()) {
            cancel_event = py::module_::import("threading").attr("Event")();
        }
        else {
            cancel_event.attr("clear")();
        }

        if (cancel_event.attr("is_set")().cast<bool>()) return;

        py::module_ module = py::module_::import(moduleName.c_str());

        if (cancel_event.attr("is_set")().cast<bool>()) return;

        py::object function = module.attr(funcName.c_str());

        if (cancel_event.attr("is_set")().cast<bool>()) return;

        consoleComponent.appendText("Preparing Python arguments...");

        for (const auto& arg : inputArgs) {
            if (cancel_event.attr("is_set")().cast<bool>()) return;

            pyInputArgs[arg.first.c_str()] = anyToPyObject(arg.second);
        }
        pyInputArgs["cancel_event"] = cancel_event;

        if (cancel_event.attr("is_set")().cast<bool>()) return;

        consoleComponent.appendText("Executing Python function...");

        function(**pyInputArgs);

        if (cancel_event.attr("is_set")().cast<bool>()) return;

        py::module_ gc = py::module_::import("gc");
        gc.attr("collect")();

        if (cancel_event.attr("is_set")().cast<bool>()) return;

        consoleComponent.appendText("Function execution completed.");
        consoleComponent.appendText("");

        if (cancel_event.attr("is_set")().cast<bool>()) return;
    }
    catch (const py::error_already_set& e) {
        consoleComponent.appendText("Python error: " + std::string(e.what()));
    }
    catch (const std::exception& e) {
        consoleComponent.appendText("C++ error: " + std::string(e.what()));
    }
    catch (...) {
        consoleComponent.appendText("Unknown error occurred.");
    }
}

void PythonBridge::goToPythonPath() {
    juce::File pluginDirectory = PluginProcessor::getPluginDirectory();
    juce::File pythonSrcDirectory = pluginDirectory.getChildFile("python_src");

    std::filesystem::path p = std::filesystem::absolute(pythonSrcDirectory.getFullPathName().toStdString());

    std::string modulePath = p.string();
    std::replace(modulePath.begin(), modulePath.end(), '\\', '/');

    std::string pythonCode = "import sys\nsys.path.append('" + modulePath + "')";
    py::exec(pythonCode);
}

py::object PythonBridge::anyToPyObject(const std::any& value) {
    if (value.type() == typeid(int)) {
        return py::cast(std::any_cast<int>(value));
    }
    else if (value.type() == typeid(double)) {
        return py::cast(std::any_cast<double>(value));
    }
    else if (value.type() == typeid(std::string)) {
        return py::cast(std::any_cast<std::string>(value));
    }
    else if (value.type() == typeid(std::map<std::string, std::any>)) {
        auto map = std::any_cast<std::map<std::string, std::any>>(value);
        py::dict pyDict;
        for (const auto& [key, val] : map) {
            pyDict[py::cast(key)] = anyToPyObject(val);
        }
        return pyDict;
    }
    else {
        throw std::runtime_error("Unsupported type in std::any");
    }
}

void PythonBridge::callPythonTraining(std::map<std::string, std::any> inputArgs, ConsoleComponent& consoleComponent) {
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (!workerThread) {
            consoleComponent.appendText("Starting thread.");
            workerThread = std::make_unique<std::thread>(&PythonBridge::workerThreadFunction, this);
        }

        if (taskRunning) {
            consoleComponent.appendText("A function is already running. Please wait until it completes.");
            return;
        }
        taskRunning = true;
    }

    enqueueTask([=, &consoleComponent]() {
        callPythonFunction("actions", "train", inputArgs, consoleComponent);

        {
            std::lock_guard<std::mutex> lock(mutex);
            taskRunning = false;
            cancel_event = py::none();
        }
    });
}

void PythonBridge::callPythonGeneration(std::map<std::string, std::any> inputArgs, ConsoleComponent& consoleComponent, MIDIDragComponent& midiDragComponent) {
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (!workerThread) {
            consoleComponent.appendText("Starting thread.");
            workerThread = std::make_unique<std::thread>(&PythonBridge::workerThreadFunction, this);
        }

        if (taskRunning) {
            consoleComponent.appendText("A function is already running. Please wait until it completes.");
            return;
        }
        taskRunning = true;
    }

    enqueueTask([=, &consoleComponent, &midiDragComponent]() {
        callPythonFunction("generation", "generate", inputArgs, consoleComponent);

        assignResultToDragComponent(inputArgs, consoleComponent, midiDragComponent);

        {
            std::lock_guard<std::mutex> lock(mutex);
            taskRunning = false;
            cancel_event = py::none();
        }
    });
}

void PythonBridge::assignResultToDragComponent(std::map<std::string, std::any> inputArgs, ConsoleComponent& consoleComponent, MIDIDragComponent& midiDragComponent) {
    juce::String filePath = std::any_cast<std::string>(inputArgs.at("temp_midi_file_path"));

    if (cancel_event && cancel_event.attr("is_set")().cast<bool>()) return;

    juce::File processedMidiFile(filePath);
    if (processedMidiFile.exists()) {

        if (cancel_event && cancel_event.attr("is_set")().cast<bool>()) return;

        juce::MidiFile processedMidi;
        juce::FileInputStream inputStream(processedMidiFile);
        if (processedMidi.readFrom(inputStream)) {
            if (cancel_event && cancel_event.attr("is_set")().cast<bool>()) return;

            midiDragComponent.setMidiFile(filePath, processedMidi);

            if (cancel_event && cancel_event.attr("is_set")().cast<bool>()) return;

            consoleComponent.appendText("Midi file set.");

            if (cancel_event && cancel_event.attr("is_set")().cast<bool>()) return;
        }
        else {
            consoleComponent.appendText("Failed to read the processed MIDI file.");
        }
    }
    else {
        consoleComponent.appendText("Processed MIDI file does not exist.");
    }
}

void PythonBridge::cancelCurrentOperation(ConsoleComponent* consoleComponent) {
    std::lock_guard<std::mutex> lock(mutex);
    if (!taskRunning) {
        if (consoleComponent) consoleComponent->appendText("No function is currently running.");
        return;
    }

    try {
        py::gil_scoped_acquire acquire;

        if (cancel_event) {
            cancel_event.attr("set")();
        }
        if (consoleComponent) consoleComponent->appendText("Cancellation requested.");
    }
    catch (const py::error_already_set& e) {
        if (consoleComponent) consoleComponent->appendText("Python error: " + std::string(e.what()));
    }
    catch (const std::exception& e) {
        if (consoleComponent) consoleComponent->appendText("C++ error: " + std::string(e.what()));
    }
    catch (...) {
        if (consoleComponent) consoleComponent->appendText("Unknown error occurred during cancellation.");
    }
}

void PythonBridge::redirectStdout(ConsoleComponent& consoleComponent) {
    auto callback = [&consoleComponent](const std::string& output) {
        consoleComponent.appendText(output);
    };

    py::cpp_function pyCallback(callback);
    py::dict locals;
    locals["callback"] = pyCallback;

    py::exec(R"(
        import sys
        class StdoutCatcher:
            def __init__(self, callback):
                self.callback = callback

            def write(self, text):
                if text.strip() != '':
                    self.callback(text)

            def flush(self):
                pass

        sys.stdout = StdoutCatcher(callback)
        sys.stderr = StdoutCatcher(callback)
    )", py::globals(), locals);
}