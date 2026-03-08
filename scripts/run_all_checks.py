#
# link this into your macro directory
# ln -s $(pwd)/scripts/run_all_checks.py ~/.local/share/FreeCAD/Macro/
#
import FreeCAD as App
import os
import glob

doc = App.ActiveDocument
if not doc:
    print("No active document found.")
else:
    # Get the directory containing the model
    model_path = doc.FileName
    if not model_path:
        print("Model has not been saved yet. Please save the file first.")
    else:
        model_dir = os.path.dirname(model_path)
        scripts_dir = os.path.join(model_dir, "scripts")

        if not os.path.isdir(scripts_dir):
            print(f"Scripts folder not found: {scripts_dir}")
        else:
            # Find all check*.py files
            check_scripts = glob.glob(os.path.join(scripts_dir, "check*.py"))
            if not check_scripts:
                print("No check*.py scripts found in the scripts folder.")
            else:
                print(f"\nFound {len(check_scripts)} check script(s) in {scripts_dir}\n")
                for i, script_path in enumerate(sorted(check_scripts), 1):
                    script_name = os.path.basename(script_path)
                    print(f"\n--- Running check script {i}: {script_name} ---\n")
                    try:
                        # Execute the script in the current namespace
                        # (using exec with the script's full path)
                        with open(script_path, 'r') as f:
                            script_code = f.read()
                        exec(compile(script_code, script_path, 'exec'))
                    except Exception as e:
                        print(f"!!! Error in {script_name}: {e}")
                    print(f"\n--- Finished {script_name} ---")
                print("\nAll checks completed.")
