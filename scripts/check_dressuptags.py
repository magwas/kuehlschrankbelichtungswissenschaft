import FreeCAD as App

doc = App.ActiveDocument
if not doc:
    print("No active document found.")
else:
    tags_found = False
    for obj in doc.Objects:
        # Check if it's a CAM operation (Path or CAM workbench)
        if hasattr(obj, "TypeId") and (obj.TypeId.startswith("Path::") or obj.TypeId.startswith("CAM::")):
            # Try to get the operation type from the Proxy (like pocket script)
            op_type = None
            if hasattr(obj, "Proxy") and obj.Proxy:
                proxy_class = obj.Proxy.__class__.__name__
                op_type = proxy_class
            elif hasattr(obj, "OperationType"):
                op_type = obj.OperationType
            elif hasattr(obj, "OpType"):
                op_type = obj.OpType

            # If we found a dressup tag (case‑insensitive "tag" in op_type)
            if op_type and "tag" in op_type.lower():
                tags_found = True

                # --- Retrieve width, height, angle ---
                # Width
                width_val = None
                for prop in ["Width", "TagWidth", "width", "tagWidth"]:
                    if hasattr(obj, prop):
                        width_val = getattr(obj, prop)
                        break
                # Height
                height_val = None
                for prop in ["Height", "TagHeight", "height", "tagHeight"]:
                    if hasattr(obj, prop):
                        height_val = getattr(obj, prop)
                        break
                # Angle
                angle_val = None
                for prop in ["Angle", "TagAngle", "angle", "tagAngle"]:
                    if hasattr(obj, prop):
                        angle_val = getattr(obj, prop)
                        break

                # --- Format values for display and comparison ---
                width_str = "N/A"
                if width_val is not None:
                    if isinstance(width_val, (int, float)):
                        width_str = f"{width_val} mm"
                    else:
                        width_str = str(width_val)

                height_str = "N/A"
                if height_val is not None:
                    if isinstance(height_val, (int, float)):
                        height_str = f"{height_val} mm"
                    else:
                        height_str = str(height_val)

                angle_str = "N/A"
                if angle_val is not None:
                    if isinstance(angle_val, (int, float)):
                        angle_str = f"{angle_val}°"
                    else:
                        angle_str = str(angle_val)

                # --- Print only if NOT the correct values (20 mm, 10 mm, 45°) ---
                if not (width_str == "20.0 mm" and height_str == "10.0 mm" and angle_str == "45.0 deg"):
                    print("="*50)
                    print(f"Object: {obj.Label}  (Type: {obj.TypeId})")
                    print(f"Operation Type: {op_type}")
                    print(f"Width:  {width_str}")
                    print(f"Height: {height_str}")
                    print(f"Angle:  {angle_str}")
                    print("="*50)

    if not tags_found:
        print("No DressupTag objects found in the document.")
