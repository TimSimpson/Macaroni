import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

SimpleBoostProject{
  group=upper.Group,
  project=upper.Name .. ".Enum",
  version=upper.Version,
  src="src",
  target="target",
};


local table = context.Root:Find("TrafficLights").Member:GetValues();
print("HAI")
for k, v in pairs(table) do
    print(tostring(k) .. "=" .. tostring(v));
end
