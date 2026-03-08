#
# link this into your macro directory
# ln -s $(pwd)/scripts/check_cam_operations.py ~/.local/share/FreeCAD/Macro/
#
import FreeCAD as App
import json
import os

def get_operation_params(op):
    """Must match the function in save script for consistent comparison.
    Copy the exact same function from save_cam_operations.py.
    """
    params = {
        "Label": op.Label,
        "TypeId": op.TypeId,
        "OperationType": None,
    }
    if hasattr(op, "Proxy") and op.Proxy:
        params["OperationType"] = op.Proxy.__class__.__name__
    elif hasattr(op, "OperationType"):
        params["OperationType"] = op.OperationType
    elif hasattr(op, "OpType"):
        params["OperationType"] = op.OpType

    for prop in ["ToolNumber", "ClearanceHeight", "FinalDepth", "StepOver", "OffsetPattern", "Pattern"]:
        if hasattr(op, prop):
            val = getattr(op, prop)
            if hasattr(val, "Value"):
                val = val.Value
            params[prop] = val

    if params.get("OperationType") and "Tag" in params["OperationType"]:
        for prop in ["Width", "Height", "Angle", "TagWidth", "TagHeight", "TagAngle"]:
            if hasattr(op, prop):
                val = getattr(op, prop)
                if hasattr(val, "Value"):
                    val = val.Value
                params[prop] = val
    return params

doc = App.ActiveDocument
if not doc:
    print("No active document found.")
else:
    model_path = doc.FileName
    if not model_path:
        print("Model must be saved.")
    else:
        base = os.path.splitext(model_path)[0]
        snapshot_file = base + "_cam_operations.json"
        if not os.path.exists(snapshot_file):
            print(f"Snapshot file not found: {snapshot_file}")
        else:
            with open(snapshot_file, 'r') as f:
                snapshot = json.load(f)

            # Build dictionary of current operations keyed by ObjectName
            current_ops = {}
            for obj in doc.Objects:
                if hasattr(obj, "TypeId") and (obj.TypeId.startswith("Path::") or obj.TypeId.startswith("CAM::")):
                    if hasattr(obj, "Proxy") or hasattr(obj, "OperationType") or hasattr(obj, "OpType"):
                        op_data = {
                            "ObjectName": obj.Name,
                            "Label": obj.Label,
                            "TypeId": obj.TypeId,
                            "Parameters": get_operation_params(obj)
                        }
                        current_ops[obj.Name] = op_data

            # Compare
            snapshot_ops = {op["ObjectName"]: op for op in snapshot["Operations"]}

            removed = [name for name in snapshot_ops if name not in current_ops]
            added = [name for name in current_ops if name not in snapshot_ops]
            common = [name for name in snapshot_ops if name in current_ops]

            changes = []
            for name in common:
                if snapshot_ops[name]["Parameters"] != current_ops[name]["Parameters"]:
                    changes.append(name)

            print("\n=== CAM Operations Comparison ===\n")
            if removed:
                print("❌ Removed operations:")
                for name in removed:
                    print(f"   - {snapshot_ops[name]['Label']} ({name})")
            if added:
                print("➕ Added operations:")
                for name in added:
                    print(f"   + {current_ops[name]['Label']} ({name})")
            if changes:
                print("✏️ Changed operations:")
                for name in changes:
                    old = snapshot_ops[name]["Parameters"]
                    new = current_ops[name]["Parameters"]
                    print(f"   * {snapshot_ops[name]['Label']} ({name})")
                    # Show parameter differences
                    all_keys = set(old.keys()) | set(new.keys())
                    for key in sorted(all_keys):
                        if old.get(key) != new.get(key):
                            print(f"       - {key}: {old.get(key)} → {new.get(key)}")
            if not (removed or added or changes):
                print("✅ No differences found. All operations match the snapshot.")

            print("\nComparison complete.")
