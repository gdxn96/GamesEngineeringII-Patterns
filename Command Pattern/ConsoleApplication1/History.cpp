#include "stdafx.h"
#include "History.h"
#include "Command.h"



History::History() : lastExecuted(-1), lastSaved(-1) 
{
}


History::~History() {
    for ( auto command : history ) 
	{
        delete command;
    }
}


void History::clear() {
    for ( auto command : history ) 
	{
        delete command;
    }
    history.clear();
    lastExecuted = -1;
    lastSaved = -1;
}


void History::save() 
{
    lastSaved = lastExecuted;
}


bool History::modified() 
{
    return lastSaved != lastExecuted;
}


void History::limit( unsigned int numCommands ) 
{
    while ( history.size() > numCommands ) 
	{
        delete history[ 0 ];
        history.erase( history.begin() );
        if ( lastExecuted >= 0 ) 
		{
            lastExecuted--;
        }
        if ( lastSaved >= 0 ) 
		{
            lastSaved--;
        }
    }
}


void History::add( Command* command, bool execute ) 
{
    // Remove all commands in the branch that is "cut off" by adding a new command after undo.
    if ( lastExecuted + 1 < history.size() ) 
	{
        int count = (int)history.size() - ( lastExecuted + 1 );
        int begin = lastExecuted + 1;
        int end = begin + count;
        
        for ( size_t i=0; i < count; i++ ) 
		{
            delete history[ begin + i ];
        }
        history.erase( history.begin() + begin, history.begin() + end );
        lastSaved = -1;
    }

    if ( execute ) 
	{
        command->execute();
    }

    history.push_back( command );
    lastExecuted = (int)history.size() - 1;
}


void History::revert() 
{
    while ( lastExecuted > 0 ) 
	{
        history[ lastExecuted ]->undo();
        lastExecuted--;
    }
}


void History::undo() 
{
    if ( lastExecuted >= 0 ) 
	{
        if ( history.size() > 0 ) 
		{
            history[ lastExecuted ]->undo();
            lastExecuted--;
        }
    }
}


void History::redo() 
{
    if ( lastExecuted + 1 < history.size() ) 
	{
        history[ lastExecuted + 1 ]->execute();
        lastExecuted++;
    }
}