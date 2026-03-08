# -*- coding: utf-8 -*-
import FreeCAD
import FreeCADGui
import os
import subprocess

def export_jobs_to_relative_folder():
    doc = FreeCAD.ActiveDocument
    if not doc:
        FreeCAD.Console.PrintError("No active document.\n")
        return

    if not doc.FileName:
        FreeCAD.Console.PrintError("Document must be saved first.\n")
        return

    doc.recompute()

    doc_dir = os.path.dirname(doc.FileName)
    jobs_dir = os.path.join(doc_dir, "jobs")
    if not os.path.exists(jobs_dir):
        os.makedirs(jobs_dir)
        FreeCAD.Console.PrintMessage(f"Created folder: {jobs_dir}\n")

    # Find all CAM jobs
    jobs = [obj for obj in doc.Objects if obj.TypeId == "Path::FeaturePython" and obj.Name.startswith("Job")]

    if not jobs:
        FreeCAD.Console.PrintWarning("No CAM Jobs found.\n")
        return

    original_paths = {}

    try:
        for job in jobs:
            safe_label = "".join(c for c in job.Label if c.isalnum() or c in (' ', '-', '_')).rstrip()
            if not safe_label:
                safe_label = job.Name
            output_file = os.path.join(jobs_dir, safe_label + ".gcode")
            FreeCAD.Console.PrintMessage(f"Exporting {job.Label} ({job.Name}) to {output_file}\n")

            original_paths[job.Name] = job.PostProcessorOutputFile
            job.PostProcessorOutputFile = output_file

            FreeCADGui.Selection.clearSelection()
            FreeCADGui.Selection.addSelection(job)
            FreeCADGui.runCommand('CAM_Post', 0)   # Uses dialog – you must click OK

            FreeCAD.Console.PrintMessage(f"  Successfully exported {job.Label}\n")

    except Exception as e:
        FreeCAD.Console.PrintError(f"Error during export: {e}\n")
    finally:
        for job_name, orig_path in original_paths.items():
            job_obj = doc.getObject(job_name)
            if job_obj:
                job_obj.PostProcessorOutputFile = orig_path

    FreeCAD.Console.PrintMessage("All jobs exported.\n")

    # --- Delete the stray "-" file if it exists ---
    dash_file = os.path.join(jobs_dir, "-")
    if os.path.exists(dash_file):
        try:
            os.remove(dash_file)
            FreeCAD.Console.PrintMessage(f"Removed stray '-` file from {jobs_dir}\n")
        except Exception as e:
            FreeCAD.Console.PrintError(f"Failed to remove '-` file: {e}\n")

    # --- Run fixjobs.sh in the jobs directory ---
    try:
        os.chdir(jobs_dir)
        script_path = "./fixjobs.sh"
        if not os.path.isfile(script_path):
            FreeCAD.Console.PrintError(f"fixjobs.sh not found in {jobs_dir}\n")
            return
        if not os.access(script_path, os.X_OK):
            FreeCAD.Console.PrintWarning("fixjobs.sh is not executable. Attempting to run anyway.\n")
        result = subprocess.run([script_path], capture_output=True, text=True)
        if result.returncode == 0:
            FreeCAD.Console.PrintMessage("fixjobs.sh ran successfully.\n")
            if result.stdout:
                FreeCAD.Console.PrintMessage("Output:\n" + result.stdout)
        else:
            FreeCAD.Console.PrintError(f"fixjobs.sh failed with code {result.returncode}\n")
            if result.stderr:
                FreeCAD.Console.PrintError("Error:\n" + result.stderr)
    except Exception as e:
        FreeCAD.Console.PrintError(f"Failed to run fixjobs.sh: {e}\n")

# Run the macro
export_jobs_to_relative_folder()
