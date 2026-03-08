#
# link this into your macro directory
# ln -s $(pwd)/scripts/check_tools.py ~/.local/share/FreeCAD/Macro/
#
import FreeCAD as App

def get_prop_value(obj, possible_names):
    """Return display string for a property."""
    if obj is None:
        return "N/A"
    for name in possible_names:
        if hasattr(obj, name):
            prop = getattr(obj, name)
            # If it's a Quantity with unit
            if hasattr(prop, "getUserPreferred"):
                return prop.getUserPreferred()
            # Otherwise, just return the string representation
            return str(prop)
    return "N/A"

def collect_tools_from_group(group_obj):
    """Given a group object (e.g., a Tools group), return contained tool controllers."""
    tools = []
    if hasattr(group_obj, "Group") and hasattr(group_obj.Group, "__iter__"):
        for item in group_obj.Group:
            if hasattr(item, "Proxy") and item.Proxy and item.Proxy.__class__.__name__ == "ToolController":
                tools.append(item)
    return tools

doc = App.ActiveDocument
if not doc:
    print("No active document found.")
else:
    # Identify jobs (Proxy class contains "Job")
    jobs = []
    for obj in doc.Objects:
        if hasattr(obj, "TypeId") and (obj.TypeId.startswith("Path::") or obj.TypeId.startswith("CAM::")):
            if hasattr(obj, "Proxy") and obj.Proxy:
                proxy_name = obj.Proxy.__class__.__name__
                if "Job" in proxy_name:
                    jobs.append(obj)

    if not jobs:
        print("No CAM jobs found.")
    else:
        any_bad = False
        for job in jobs:
            # Get actual tool controllers from job.Tools group
            job_tools = []
            if hasattr(job, "Tools") and job.Tools is not None:
                job_tools = collect_tools_from_group(job.Tools)

            for tool in job_tools:
                tool_label = tool.Label if hasattr(tool, "Label") else "Unnamed Tool"
                horiz = get_prop_value(tool, ["HorizFeed", "HorizontalFeed", "FeedRate"])
                vert = get_prop_value(tool, ["VertFeed", "VerticalFeed", "FeedRate"])
                spindle = get_prop_value(tool, ["SpindleSpeed", "Spindle"])

                # Expected values: horizontal=10, vertical=10, spindle=100 (as numbers)
                # Convert displayed strings back to numbers? Simpler: check the raw numeric values.
                # We'll re‑fetch raw numbers for comparison.
                def get_raw(obj, names):
                    for name in names:
                        if hasattr(obj, name):
                            prop = getattr(obj, name)
                            if hasattr(prop, "Value"):
                                return prop.Value
                            elif isinstance(prop, (int, float)):
                                return prop
                    return None

                horiz_raw = get_raw(tool, ["HorizFeed", "HorizontalFeed", "FeedRate"])
                vert_raw = get_raw(tool, ["VertFeed", "VerticalFeed", "FeedRate"])
                spindle_raw = get_raw(tool, ["SpindleSpeed", "Spindle"])

                if not (horiz_raw == 10 and vert_raw == 10 and spindle_raw == 100):
                    any_bad = True
                    print(f"Job: {job.Label} | Tool: {tool_label}")
                    print(f"  Horizontal Feed: {horiz}")
                    print(f"  Vertical Feed:   {vert}")
                    print(f"  Spindle Speed:   {spindle}")
                    print("-" * 40)

        if not any_bad:
            print("✅ All tools have the correct feed rates and spindle speed (10, 10, 100).")
