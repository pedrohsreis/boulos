#include "Module.h"
#include "utils/Timer.hpp"
#include "Utils/Math.h"
#include <iostream>

using namespace std;

#define MAX(v1, v2) (v1 > v2 ? v1 : v2)
#define MIN(v1, v2) (v1 < v2 ? v1 : v2)

Module::Module(SpellBook *spellBook, std::string name, int ms)
{   
    is_running = false;
    highPriority = false;
    this->name = name;
    this->us = ms*1000;
    this->spellBookBase = spellBook;
    this->spellBook = new SpellBook();
}

Module::~Module()
{
    delete spellBook;
}

void Module::SetHighPriority(bool highPriority)
{
    this->highPriority = highPriority;
}

bool Module::IsRunning()
{
    return is_running;
}

bool Module::IsHighPriority()
{
    return highPriority;
}

string Module::Name()
{
    return name;
}

void Module::Tick(float ellapsedTime)
{
    
}

void Module::OnStop()
{

}

void Module::OnStart()
{

}

void Module::Load()
{

}

void Module::Save()
{

}

void Module::Start()
{
    cout << "Starting " << name << endl;
    is_running = true;
    OnStart();
    int rc;
    if(!highPriority)
    {
        rc = pthread_create(&thread, NULL, Module::Run, (void *)this); //rc = returned code
    }
    else
    {
        struct sched_param param;
        int policy;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
        param.sched_priority = 65;
        pthread_attr_setschedparam(&attr, &param);
        pthread_create(&thread, &attr, Module::Run, (void *)this);
        pthread_attr_destroy(&attr);
        pthread_getschedparam(thread, &policy, &param);
    }
    cout << name << " started" << endl;
}

void Module::Stop()
{
    cout << "Stopping " << name << endl;
    is_running = false;
    Join();
    cout << name << " stopped" << endl;
}

void Module::Join()
{
    pthread_join(thread, NULL);
    OnStop();
}

void *Module::Run(void *arg)
{
    Module *module = (Module *)arg;
    Timer timer, timerWait;
    float t;
    int us = 0, wt = 0;
    while (module->is_running)
    {
        t = timer.elapsed_us() * 0.000001f;
        timer.restart();
        //t = timer.elapsed();
        if(t < 0.0f)
            t = -t;
        timerWait.restart();
        module->Load();
        module->Tick(t);
        module->Save();
        us = timerWait.elapsed_us();
        
        wt = min(max(module->us - us, 50), 5000000);
        //cout << module->Name() << " -> " << module->us << " - " << us << " = " << wt << endl;
        usleep(wt);
    }
    pthread_exit(NULL);
}

InnerModule::InnerModule(SpellBook *spellBook)
{
    this->spellBook = spellBook;
}

InnerModule::~InnerModule()
{
    
}

void InnerModule::Tick(float ellapsedTime)
{
    
}

void InnerModule::OnStop()
{

}

void InnerModule::OnStart()
{

}