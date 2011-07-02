--------------------------------------------------------------------------------
-- Copyright 2011 Tim Simpson
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--    http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.
--------------------------------------------------------------------------------
require "Macaroni.Model.Context";
require "Macaroni.Model.Scope";

local Context = Macaroni.Model.Context;
local Scope = Macaroni.Model.Scope;

Test.register(
{	
name = "Scope Tests-Static Functions",    

tests={	       

    ["IsComplexName returns false for blanks."] = function(this)                
		Test.assert(false, Scope.IsComplexName(""));
    end, 
    
    ["IsComplexName returns false for simple names."] = function(this)                
		Test.assert(false, Scope.IsComplexName("A"));
		Test.assert(false, Scope.IsComplexName("Animal"));
		Test.assert(false, Scope.IsComplexName("Doggy42"));
    end, 
    
    ["IsComplexName returns true in the presense of ::."] = function(this)                
		Test.assert(true, Scope.IsComplexName("::"));
		Test.assert(true, Scope.IsComplexName("A::B"));
		Test.assert(true, Scope.IsComplexName("Organisms::Animals::Dog"));
    end, 
    
    ["IsSimpleName returns true for blanks."] = function(this)                
		Test.assert(true, Scope.IsSimpleName(""));		
    end, 
    
    ["IsSimpleName returns true for simple names."] = function(this)                
		Test.assert(true, Scope.IsSimpleName("A"));		
		Test.assert(true, Scope.IsSimpleName("Doggy"));		
		Test.assert(true, Scope.IsSimpleName("DogCatDemonHide"));		
    end, 
    
    ["IsSimpleName returns false when faced with ::."] = function(this)                
		Test.assert(false, Scope.IsSimpleName("A::B"));		
		Test.assert(false, Scope.IsSimpleName("Animals::Doggy"));		
		Test.assert(false, Scope.IsSimpleName("Ryu::Enemies::DogCatDemonHide"));		
    end, 
    
    ["ParseComplexName creates nothing if given a blank."] = function(this)
        local context = Context.New("{ROOT}", "{WILDCARD}");
        local resultScope, resultName = 
            Scope.ParseComplexName(context.RootNamespace, "");
        Test.assert("", resultName);
        Test.assert(0, #context.RootNamespace.Members);
    end,
        
    ["ParseComplexName creates nothing if given a simple name."] = function(this)
        local context = Context.New("{ROOT}", "{WILDCARD}");
        local resultScope, resultName = 
            Scope.ParseComplexName(context.RootNamespace, "Doggy");
        Test.assert("Doggy", resultName);
        Test.assert(0, #context.RootNamespace.Members);
    end,
    
    ["ParseComplexName creates UnknownScope if given a complex name."] = function(this)
        local context = Context.New("{ROOT}", "{WILDCARD}");
        local resultScope, resultName = 
            Scope.ParseComplexName(context.RootNamespace, "Animals::Doggy");
        Test.assert("Doggy", resultName);
        Test.assert(1, #(context.RootNamespace.Members));
        Test.assert("Animals", resultScope.Name);
        Test.assert(context.RootNamespace.Members[1], resultScope);
        Test.assert(0, #(resultScope.Members));
    end,
    
    ["ParseComplexName creates multiple UnknownScope if needed."] = function(this)
        local context = Context.New("{ROOT}", "{WILDCARD}");
        local resultScope, resultName = 
            Scope.ParseComplexName(context.RootNamespace, "Organisms::Animals::Kitty");
        Test.assert("Kitty", resultName);
        Test.assert(1, #(context.RootNamespace.Members));
        local organisms = context.RootNamespace.Members[1];
        Test.assert("Organisms", organisms.Name);        
        Test.assert(1, #(organisms.Members));
        Test.assert("Animals", context.RootNamespace.Members[1].Members[1].Name);
        Test.assert(0, #(context.RootNamespace.Members[1].Members[1].Members));
        Test.assert(context.RootNamespace.Members[1].Members[1], resultScope);
        Test.assert("Organisms::Animals", resultScope.FullName);
    end,
    
    ["SplitFirstNameOffComplexName returns nothing for nothing."] = function(this)        
        local firstPart, lastPart = Scope.SplitFirstNameOffComplexName("")        
		Test.assert("", firstPart);
		Test.assert("", lastPart);
    end, 
    
    ["SplitFirstNameOffComplexName returns only first part when no :: are found."] = function(this)        
        local firstPart, lastPart = Scope.SplitFirstNameOffComplexName("Doggy")        
		Test.assert("Doggy", firstPart);
		Test.assert("", lastPart);
    end, 
    
    ["SplitFirstNameOffComplexName returns simple string when some stuff after :: is found."] = function(this)        
        local firstPart, lastPart = Scope.SplitFirstNameOffComplexName("Animal::Doggy")        
		Test.assert("Animal", firstPart);
		Test.assert("Doggy", lastPart);
    end, 
    
    ["SplitFirstNameOffComplexName returns lots of stuff if found after ::."] = function(this)        
        local firstPart, lastPart = Scope.SplitFirstNameOffComplexName("Organisms::Animal::Doggy")        
		Test.assert("Organisms", firstPart);
		Test.assert("Animal::Doggy", lastPart);
    end, 
    
    ["SplitScopeAndMemberName scope is blank, member is blank if nothing given."] = function(this)        
        local scope, member = Scope.SplitScopeAndMemberName("")
		Test.assert("", scope);
		Test.assert("", member);
    end, 
    
    ["SplitScopeAndMemberName scope is blank if no :: found."] = function(this)        
        local scope, member = Scope.SplitScopeAndMemberName("a")
		Test.assert("", scope);
		Test.assert("a", member);
    end,
    
    ["SplitScopeAndMemberName works with one :: found."] = function(this)
        local scopeName, memberName = Scope.SplitScopeAndMemberName("b::a")
        Test.assert("b", scopeName);
        Test.assert("a", memberName);
    end,
    
    ["SplitScopeAndMemberName works with two :: found."] = function(this)
        local scopeName, memberName = Scope.SplitScopeAndMemberName("Arg::big::Orange")
        Test.assert("Arg::big", scopeName);
        Test.assert("Orange", memberName);
    end,
    
    ["SplitScopeAndMemberName works with three (or lets presume more) :: found."] = function(this)
        local scopeName, memberName = Scope.SplitScopeAndMemberName("King::Darius::Gradius::Vic")
        Test.assert("King::Darius::Gradius", scopeName);
        Test.assert("Vic", memberName);
    end,   
    
    ["SplitComplexName returns zero elements for blank."] = function(this)
        local names = Scope.SplitComplexName("")
        Test.assert({}, names);        
    end,   
    
    ["SplitComplexName returns one element for single names."] = function(this)
        local names = Scope.SplitComplexName("Porcupine")
        Test.assert({"Porcupine"}, names);        
    end,   
    
    ["SplitComplexName returns 2 elements when it needs to."] = function(this)
        local names = Scope.SplitComplexName("Animals::Porcupine")
        Test.assert({"Animals", "Porcupine"}, names);        
    end,   
    
    ["SplitComplexName returns 3 or more elements when it needs to."] = function(this)
        local names = Scope.SplitComplexName("Organisms::Animals::Porcupine")
        Test.assert({"Organisms", "Animals", "Porcupine"}, names);        
    end,   
}

} -- End of Test table.
); -- End of register call
