/*
  ==============================================================================

    PythonBridge.h
    Created: 12 May 2024 10:44:41am
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h> // pybind11 for embedding Python
#include <pybind11/stl.h>   // pybind11 for handling standard C++ containers
#include <pybind11/functional.h>

#include <condition_variable>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <filesystem>
#include <algorithm>
#include <any>

#include "ConsoleComponent.h"
#include "MIDIDragComponent.h"

//==============================================================================
/*
*/

namespace py = pybind11;

class PythonBridge {
public:
	PythonBridge();
	~PythonBridge();

    void kill();
private:
    void init();
    void finalize();

    void workerThreadFunction();
    void enqueueTask(const std::function<void()>&task);

private:
    void callPythonFunction(std::string moduleName, std::string funcName, std::map<std::string, std::any> inputArgs, ConsoleComponent& consoleComponent);
    void goToPythonPath();
    py::object PythonBridge::anyToPyObject(const std::any& value);

public :
    void callPythonTraining(std::map<std::string, std::any> inputArgs, ConsoleComponent& consoleComponent);
    void callPythonGeneration(std::map<std::string, std::any> inputArgs, ConsoleComponent& consoleComponent, MIDIDragComponent& midiDragComponent);
    void assignResultToDragComponent(std::map<std::string, std::any> inputArgs, ConsoleComponent& consoleComponent, MIDIDragComponent& midiDragComponent);
    void cancelCurrentOperation(ConsoleComponent* consoleComponent = nullptr);
    void redirectStdout(ConsoleComponent& consoleComponent);

public :
    std::atomic<bool> taskRunning{ false };
    std::unique_ptr <std::thread> workerThread;
    std::queue<std::function<void()>> tasks;

private:
    std::condition_variable condition;

    py::object cancel_event;

    std::mutex mutex;
    std::atomic<bool> threadRunning{ false };
    std::atomic<bool> cancelRequested{ false };

    std::thread taskThread;
};
