import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

SimpleBoostProject{
  group=upper.Group,
  project=upper.Name .. ".Enum",
  version=upper.Version,
  src="src",
  docs="docs",
  target="target",
};


--MDOC-BEGIN:example
print("Here's how you print out the enum values:")
local table = context.Root:Find("traffic_lights").Member:GetValues();
for k, v in pairs(table) do
    print(tostring(k) .. "=" .. tostring(v));
end
--MDOC-END:example
