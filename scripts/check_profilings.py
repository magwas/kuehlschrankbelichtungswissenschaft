#
# link this into your macro directory
# ln -s $(pwd)/scripts/check_profile_depth.py ~/.local/share/FreeCAD/Macro/
#
import FreeCAD as App
import re

def get_raw_depth(obj):
    """Return raw numeric depth value from common property names."""
    for name in ["FinalDepth", "Depth", "StepDown"]:
        if hasattr(obj, name):
            prop = getattr(obj, name)
            if hasattr(prop, "Value"):
                return prop.Value
            elif isinstance(prop, (int, float)):
                return prop
            elif isinstance(prop, tuple) and len(prop) >= 1:
                match = re.match(r"([-+]?\d*\.?\d+)", str(prop[0]))
                if match:
                    return float(match.group(1))
    return None

def get_depth_display(obj):
    """Return formatted depth string with unit."""
    for name in ["FinalDepth", "Depth", "StepDown"]:
        if hasattr(obj, name):
            prop = getattr(obj, name)
            if hasattr(prop, "getUserPreferred"):
                return prop.getUserPreferred()
            elif isinstance(prop, tuple) and len(prop) >= 1:
                return prop[0]
            else:
                return str(prop)
    return "N/A"

def is_profile_operation(obj):
    """Return True if obj is a profile operation based on its proxy class name."""
    if not hasattr(obj, "Proxy") or not obj.Proxy:
        return False
    proxy_name = obj.Proxy.__class__.__name__
    return "profile" in proxy_name.lower()

def is_job(obj):
    """Return True if obj is a CAM job."""
    if not hasattr(obj, "Proxy") or not obj.Proxy:
        return False
    proxy_name = obj.Proxy.__class__.__name__
    return "Job" in proxy_name

def find_job_for_operation(op, visited=None):
    """Traverse upwards via InList to find a job that contains this operation."""
    if visited is None:
        visited = set()
    if op in visited:
        return None
    visited.add(op)

    # Check if op itself is a job (unlikely)
    if is_job(op):
        return op

    # Check if op has a Job attribute
    if hasattr(op, "Job"):
        job_attr = getattr(op, "Job")
        if job_attr and is_job(job_attr):
            return job_attr

    # Traverse InList
    if hasattr(op, "InList"):
        for parent in op.InList:
            if is_job(parent):
                return parent
            job = find_job_for_operation(parent, visited)
            if job:
                return job
    return None

def collect_operations_in_group(group_obj, collected=None):
    """Recursively collect all CAM operations inside a group."""
    if collected is None:
        collected = []
    if group_obj is None:
        return collected
    if hasattr(group_obj, "Group") and hasattr(group_obj.Group, "__iter__"):
        for child in group_obj.Group:
            if hasattr(child, "TypeId") and (child.TypeId.startswith("Path::") or child.TypeId.startswith("CAM::")):
                collected.append(child)
            collect_operations_in_group(child, collected)
    return collected

doc = App.ActiveDocument
if not doc:
    print("No active document found.")
else:
    # Identify all jobs
    jobs = []
    for obj in doc.Objects:
        if is_job(obj):
            jobs.append(obj)

    if not jobs:
        print("No CAM jobs found.")
    else:
        # Build mapping: operation -> job (by tree membership)
        op_to_job = {}
        for job in jobs:
            if not hasattr(job, "Operations") or job.Operations is None:
                continue
            ops_group = job.Operations
            ops = collect_operations_in_group(ops_group)
            for op in ops:
                op_to_job[op] = job

        any_bad = False
        for obj in doc.Objects:
            if not is_profile_operation(obj):
                continue

            # Determine job: first try tree mapping, then upward traversal
            job = op_to_job.get(obj, None)
            if job is None:
                job = find_job_for_operation(obj)

            job_label = job.Label if job else "Unknown Job"

            depth_raw = get_raw_depth(obj)
            depth_disp = get_depth_display(obj)

            if depth_raw is None:
                any_bad = True
                print(f"Job: {job_label} | Operation: {obj.Label} – Depth property not found")
                print("-" * 40)
            elif abs(depth_raw) > 1e-9:  # not zero (allow small floating error)
                any_bad = True
                print(f"Job: {job_label} | Operation: {obj.Label}")
                print(f"  Depth: {depth_disp}  (should be 0)")
                print("-" * 40)

        if not any_bad:
            print("✅ All profile operations have depth = 0.")
