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
		Test.assertEquals(false, Scope.IsComplexName(""));
    end, 
    
    ["IsComplexName returns false for simple names."] = function(this)                
		Test.assertEquals(false, Scope.IsComplexName("A"));
		Test.assertEquals(false, Scope.IsComplexName("Animal"));
		Test.assertEquals(false, Scope.IsComplexName("Doggy42"));
    end, 
    
    ["IsComplexName returns true in the presense of ::."] = function(this)                
		Test.assertEquals(true, Scope.IsComplexName("::"));
		Test.assertEquals(true, Scope.IsComplexName("A::B"));
		Test.assertEquals(true, Scope.IsComplexName("Organisms::Animals::Dog"));
    end, 
    
    ["IsSimpleName returns true for blanks."] = function(this)                
		Test.assertEquals(true, Scope.IsSimpleName(""));		
    end, 
    
    ["IsSimpleName returns true for simple names."] = function(this)                
		Test.assertEquals(true, Scope.IsSimpleName("A"));		
		Test.assertEquals(true, Scope.IsSimpleName("Doggy"));		
		Test.assertEquals(true, Scope.IsSimpleName("DogCatDemonHide"));		
    end, 
    
    ["IsSimpleName returns false when faced with ::."] = function(this)                
		Test.assertEquals(false, Scope.IsSimpleName("A::B"));		
		Test.assertEquals(false, Scope.IsSimpleName("Animals::Doggy"));		
		Test.assertEquals(false, Scope.IsSimpleName("Ryu::Enemies::DogCatDemonHide"));		
    end, 
    
    ["ParseComplexName creates nothing if given a blank."] = function(this)
        local context = Context.New("{ROOT}", "{WILDCARD}");
        local resultScope, resultName = 
            Scope.ParseComplexName(context.RootNamespace, "");
        Test.assertEquals("", resultName);
        Test.assertEquals(0, #context.RootNamespace.Members);
    end,
        
    ["ParseComplexName creates nothing if given a simple name."] = function(this)
        local context = Context.New("{ROOT}", "{WILDCARD}");
        local resultScope, resultName = 
            Scope.ParseComplexName(context.RootNamespace, "Doggy");
        Test.assertEquals("Doggy", resultName);
        Test.assertEquals(0, #context.RootNamespace.Members);
    end,
    
    ["ParseComplexName creates UnknownScope if given a complex name."] = function(this)
        local context = Context.New("{ROOT}", "{WILDCARD}");
        local resultScope, resultName = 
            Scope.ParseComplexName(context.RootNamespace, "Animals::Doggy");
        Test.assertEquals("Doggy", resultName);
        Test.assertEquals(1, #(context.RootNamespace.Members));
        Test.assertEquals("Animals", resultScope.Name);
        Test.assertEquals(context.RootNamespace.Members[1], resultScope);
        Test.assertEquals(0, #(resultScope.Members));
    end,
    
    ["ParseComplexName creates multiple UnknownScope if needed."] = function(this)
        local context = Context.New("{ROOT}", "{WILDCARD}");
        local resultScope, resultName = 
            Scope.ParseComplexName(context.RootNamespace, "Organisms::Animals::Kitty");
        Test.assertEquals("Kitty", resultName);
        Test.assertEquals(1, #(context.RootNamespace.Members));
        local organisms = context.RootNamespace.Members[1];
        Test.assertEquals("Organisms", organisms.Name);        
        Test.assertEquals(1, #(organisms.Members));
        Test.assertEquals("Animals", context.RootNamespace.Members[1].Members[1].Name);
        Test.assertEquals(0, #(context.RootNamespace.Members[1].Members[1].Members));
        Test.assertEquals(context.RootNamespace.Members[1].Members[1], resultScope);
        Test.assertEquals("Organisms::Animals", resultScope.FullName);
    end,
    
    ["SplitFirstNameOffComplexName returns nothing for nothing."] = function(this)        
        local firstPart, lastPart = Scope.SplitFirstNameOffComplexName("")        
		Test.assertEquals("", firstPart);
		Test.assertEquals("", lastPart);
    end, 
    
    ["SplitFirstNameOffComplexName returns only first part when no :: are found."] = function(this)        
        local firstPart, lastPart = Scope.SplitFirstNameOffComplexName("Doggy")        
		Test.assertEquals("Doggy", firstPart);
		Test.assertEquals("", lastPart);
    end, 
    
    ["SplitFirstNameOffComplexName returns simple string when some stuff after :: is found."] = function(this)        
        local firstPart, lastPart = Scope.SplitFirstNameOffComplexName("Animal::Doggy")        
		Test.assertEquals("Animal", firstPart);
		Test.assertEquals("Doggy", lastPart);
    end, 
    
    ["SplitFirstNameOffComplexName returns lots of stuff if found after ::."] = function(this)        
        local firstPart, lastPart = Scope.SplitFirstNameOffComplexName("Organisms::Animal::Doggy")        
		Test.assertEquals("Organisms", firstPart);
		Test.assertEquals("Animal::Doggy", lastPart);
    end, 
    
    ["SplitScopeAndMemberName scope is blank, member is blank if nothing given."] = function(this)        
        local scope, member = Scope.SplitScopeAndMemberName("")
		Test.assertEquals("", scope);
		Test.assertEquals("", member);
    end, 
    
    ["SplitScopeAndMemberName scope is blank if no :: found."] = function(this)        
        local scope, member = Scope.SplitScopeAndMemberName("a")
		Test.assertEquals("", scope);
		Test.assertEquals("a", member);
    end,
    
    ["SplitScopeAndMemberName works with one :: found."] = function(this)
        local scopeName, memberName = Scope.SplitScopeAndMemberName("b::a")
        Test.assertEquals("b", scopeName);
        Test.assertEquals("a", memberName);
    end,
    
    ["SplitScopeAndMemberName works with two :: found."] = function(this)
        local scopeName, memberName = Scope.SplitScopeAndMemberName("Arg::big::Orange")
        Test.assertEquals("Arg::big", scopeName);
        Test.assertEquals("Orange", memberName);
    end,
    
    ["SplitScopeAndMemberName works with three (or lets presume more) :: found."] = function(this)
        local scopeName, memberName = Scope.SplitScopeAndMemberName("King::Darius::Gradius::Vic")
        Test.assertEquals("King::Darius::Gradius", scopeName);
        Test.assertEquals("Vic", memberName);
    end,   
    
    ["SplitComplexName returns zero elements for blank."] = function(this)
        local names = Scope.SplitComplexName("")
        Test.assertEquals({}, names);        
    end,   
    
    ["SplitComplexName returns one element for single names."] = function(this)
        local names = Scope.SplitComplexName("Porcupine")
        Test.assertEquals({"Porcupine"}, names);        
    end,   
    
    ["SplitComplexName returns 2 elements when it needs to."] = function(this)
        local names = Scope.SplitComplexName("Animals::Porcupine")
        Test.assertEquals({"Animals", "Porcupine"}, names);        
    end,   
    
    ["SplitComplexName returns 3 or more elements when it needs to."] = function(this)
        local names = Scope.SplitComplexName("Organisms::Animals::Porcupine")
        Test.assertEquals({"Organisms", "Animals", "Porcupine"}, names);        
    end,   
}

} -- End of Test table.
); -- End of register call
