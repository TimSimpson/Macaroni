dfgfdclass Thing
  name: "unknown"

class Person extends Thing
  say_name: => print "CMAKE 2 Hello, I am", @name

with Person!
  .name = "MoonScript"
  \say_name!

export GetMethod
GetMethod = (name) ->
    if name == "Generate" then
        return {
            Describe: (args) ->
                args.output/WriteLine("Moon test generator.")
            ,
            Run: (args) ->
                args.output/WriteLine("HI!")
        }
    else
        nil


-- --riley
-- print 10
-- print pee
-- hi = pee - 10
