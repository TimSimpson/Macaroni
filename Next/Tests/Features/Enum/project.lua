import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"


SimpleBoostProject{
  group="Macaroni",
  project="Macaroni.Tests.Features.Enum",
  version="DEV",
  src="src",
  docs="docs",
  target="target",
};


oldDocument  = document

function document()
--MDOC-BEGIN:example
    print("Here's how you print out the enum values:")
    local table = context.Root:Find("traffic_lights").Member:GetValues();
    for k, v in pairs(table) do
        print(tostring(k) .. "=" .. tostring(v));
    end
--MDOC-END:example
    oldDocument()
end
