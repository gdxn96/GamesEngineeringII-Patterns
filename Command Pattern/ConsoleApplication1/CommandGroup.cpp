#include "stdafx.h"
#include "CommandGroup.h"


CommandGroup::~CommandGroup() 
{
    for ( auto command : commands ) 
	{
        delete command;
    }
}

void CommandGroup::add( Command* command ) 
{
    commands.push_back( command );
}


void CommandGroup::execute() 
{
    for ( unsigned int i=0; i < commands.size(); i++ ) 
	{
        commands[ i ]->execute();
    }
}


void CommandGroup::undo() 
{
    for ( unsigned int i=0; i < commands.size(); i++ ) 
	{
        commands[ i ]->undo();
    }
}

