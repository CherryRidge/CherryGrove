﻿#define V8_COMPRESS_POINTERS 1
#define V8_31BIT_SMIS_ON_64BIT_ARCH 1
#define V8_ENABLE_SANDBOX 1
#ifdef DEBUG
    #define V8_ENABLE_CHECKS 1
#endif
#include <v8.h>
#include <libplatform/libplatform.h>
#include <v8-context.h>
#include <v8-initialization.h>
#include <v8-isolate.h>
#include <v8-local-handle.h>
#include <v8-primitive.h>
#include <v8-script.h>

#include "../../debug/Logger.hpp"
#include "V8Wrapper.hpp"

using std::unique_ptr;

namespace V8Wrapper {
    static void loadFile(const char* filePath);
    v8::Isolate* V8Isolate;
    v8::Isolate::CreateParams createParams;
    unique_ptr<v8::Platform> platform;

    void init(const char* executionPath) {
        v8::V8::InitializeICUDefaultLocation(executionPath);
        v8::V8::InitializeExternalStartupData(executionPath);
        platform = v8::platform::NewDefaultPlatform();
        v8::V8::InitializePlatform(platform.get());
        v8::V8::Initialize();
        createParams.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
        V8Isolate = v8::Isolate::New(createParams);
        evalToStdout(JSENGINE_TEST);
    }

    void shutdown() {
        V8Isolate->Dispose();
        v8::V8::Dispose();
        v8::V8::DisposePlatform();
        delete createParams.array_buffer_allocator;
    }

    void evalToStdout(const char* expression) {
        v8::Isolate::Scope isolate_scope(V8Isolate);
        v8::HandleScope handle_scope(V8Isolate);
        v8::Local<v8::Context> context = v8::Context::New(V8Isolate);
        v8::Context::Scope context_scope(context);
        v8::MaybeLocal<v8::String> source = v8::String::NewFromUtf8(V8Isolate, expression, v8::NewStringType::kNormal);
        v8::Local<v8::String> local;
        if (source.ToLocal(&local)) {
            v8::Local<v8::Script> script = v8::Script::Compile(context, local).ToLocalChecked();
            v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
            v8::String::Utf8Value utf8(V8Isolate, result);
            lout << *utf8 << endl;
        }
        else lerr << "[V8Wrapper] Expression evaluation failed: " << expression << endl;
    }

    static void loadFile(const char* filePath) {
        lout << "Load file todo" << endl;
    }
}