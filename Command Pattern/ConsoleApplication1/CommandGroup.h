#pragma once
#include <vector>
#include "Command.h"

class CommandGroup : public Command 
{
private:
    std::vector<Command*> commands;

public:
    ~CommandGroup();
    void add( Command* command );
    void execute();
    void undo();
    
    
};
