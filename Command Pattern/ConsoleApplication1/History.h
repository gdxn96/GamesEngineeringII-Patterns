#pragma once
#include <vector>
class Command;

class History
{
private:
    std::vector<Command*> history;
    int lastExecuted;
    int lastSaved;
    
public:
    History();
    ~History();
    
    void clear();
    void save();
    bool modified();
    void limit( unsigned int numCommands );
    void add( Command* command, bool execute );
    void revert();
    void undo();
    void redo();
    
};
