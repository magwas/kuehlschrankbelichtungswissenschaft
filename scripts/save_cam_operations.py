#
# link this into your macro directory
# ln -s $(pwd)/scripts/save_cam_operations.py ~/.local/share/FreeCAD/Macro/
#
import FreeCAD as App
import json
import os
from datetime import datetime

def get_operation_params(op):
    """Extract relevant parameters from a CAM operation object.
    Customize this function to include the parameters you care about.
    """
    params = {
        "Label": op.Label,
        "TypeId": op.TypeId,
        "OperationType": None,
    }
    # Try to get operation type from Proxy (most reliable)
    if hasattr(op, "Proxy") and op.Proxy:
        params["OperationType"] = op.Proxy.__class__.__name__
    elif hasattr(op, "OperationType"):
        params["OperationType"] = op.OperationType
    elif hasattr(op, "OpType"):
        params["OperationType"] = op.OpType

    # Common parameters – add or remove as needed
    for prop in ["ToolNumber", "ClearanceHeight", "FinalDepth", "StepOver", "OffsetPattern", "Pattern"]:
        if hasattr(op, prop):
            val = getattr(op, prop)
            # Convert App::PropertyX to Python types
            if hasattr(val, "Value"):
                val = val.Value
            params[prop] = val

    # For DressupTag, include dimensions
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
        print("Model must be saved first.")
    else:
        base = os.path.splitext(model_path)[0]
        out_file = base + "_cam_operations.json"

        # Collect all CAM operations
        all_ops = []
        for obj in doc.Objects:
            # Include only objects that are likely CAM operations
            if hasattr(obj, "TypeId") and (obj.TypeId.startswith("Path::") or obj.TypeId.startswith("CAM::")):
                # Filter out non‑operation objects (like tool controllers) by checking for typical operation attributes
                if hasattr(obj, "Proxy") or hasattr(obj, "OperationType") or hasattr(obj, "OpType"):
                    op_data = {
                        "ObjectName": obj.Name,
                        "Label": obj.Label,
                        "TypeId": obj.TypeId,
                        "Parameters": get_operation_params(obj)
                    }
                    all_ops.append(op_data)

        output = {
            "Model": doc.Label,
            "FilePath": model_path,
            "Timestamp": datetime.now().isoformat(),
            "Operations": all_ops
        }

        with open(out_file, 'w') as f:
            json.dump(output, f, indent=2, default=str)
        print(f"Saved {len(all_ops)} operations to {out_file}")
