import FreeCAD as App

def is_pocket_operation(obj):
    """Return True if obj is a pocket operation based on its proxy class name."""
    if not hasattr(obj, "Proxy") or not obj.Proxy:
        return False
    proxy_name = obj.Proxy.__class__.__name__
    return "pocket" in proxy_name.lower()

doc = App.ActiveDocument
if not doc:
    print("No active document found.")
else:
    # Identify all jobs (objects whose proxy class contains "Job")
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
        any_bad_pocket = False
        for job in jobs:
            # Get the Operations group of the job
            if not hasattr(job, "Operations") or job.Operations is None:
                continue
            ops_group = job.Operations
            if not hasattr(ops_group, "Group") or not hasattr(ops_group.Group, "__iter__"):
                continue

            for op in ops_group.Group:
                if not is_pocket_operation(op):
                    continue

                # --- Extract pocket parameters ---
                offset_pattern = "N/A"
                if hasattr(op, "OffsetPattern"):
                    offset_pattern = str(op.OffsetPattern)
                elif hasattr(op, "Pattern"):
                    offset_pattern = str(op.Pattern)

                step_over = "N/A"
                if hasattr(op, "StepOver"):
                    step_over = op.StepOver
                    if isinstance(step_over, (int, float)):
                        step_over = f"{step_over} %"
                elif hasattr(op, "StepOverPercent"):
                    step_over = op.StepOverPercent
                    if isinstance(step_over, (int, float)):
                        step_over = f"{step_over} %"
                elif hasattr(op, "StepOverAbsolute"):
                    step_over = op.StepOverAbsolute

                # Print only if not default (50% and Offset)
                if not (step_over == "50 %" and offset_pattern == "Offset"):
                    any_bad_pocket = True
                    print("=" * 50)
                    print(f"Job: {job.Label}")
                    print(f"Object: {op.Label}  (Type: {op.TypeId})")
                    op_type = op.Proxy.__class__.__name__ if hasattr(op, "Proxy") and op.Proxy else "Unknown"
                    print(f"Operation Type: {op_type}")
                    print(f"Offset Pattern: {offset_pattern}")
                    print(f"Step Over: '{step_over}'")

        if not any_bad_pocket:
            print("No non‑default pocket operations found.")
