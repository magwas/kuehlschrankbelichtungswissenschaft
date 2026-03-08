#
# link this into your macro directory
# ln -s $(pwd)/scripts/check_job_output.py ~/.local/share/FreeCAD/Macro/
#
import FreeCAD as App
import os

def is_job(obj):
    """Return True if obj is a CAM job."""
    if not hasattr(obj, "Proxy") or not obj.Proxy:
        return False
    proxy_name = obj.Proxy.__class__.__name__
    return "Job" in proxy_name

def check_output_file(job):
    """Check if job.PostProcessorOutputFile matches 'jobs/<job_label>.gcode'."""
    output = getattr(job, "PostProcessorOutputFile", "")
    if not isinstance(output, str):
        return False

    # Normalise path separators to handle both \ and /
    norm_output = os.path.normpath(output)

    # Split into directory and filename
    dirname, filename = os.path.split(norm_output)

    # Expected filename: job label + ".gcode"
    expected_filename = job.Label + ".gcode"

    # Expected directory: "jobs" (case‑insensitive on Windows? We'll do exact match)
    expected_dir = "jobs"

    # Compare
    return (filename == expected_filename) and (dirname == expected_dir)

doc = App.ActiveDocument
if not doc:
    print("No active document found.")
else:
    # Find all jobs
    jobs = [obj for obj in doc.Objects if is_job(obj)]

    if not jobs:
        print("No CAM jobs found.")
    else:
        any_bad = False
        for job in jobs:
            postproc = getattr(job, "PostProcessor", "")
            output_ok = check_output_file(job)

            # Build list of issues
            issues = []
            if postproc != "marlin":
                issues.append(f"PostProcessor = '{postproc}' (should be 'marlin')")
            if not output_ok:
                output = getattr(job, "PostProcessorOutputFile", "''")
                issues.append(f"Output file = '{output}' (should be 'jobs/{job.Label}.gcode')")

            if issues:
                any_bad = True
                print("=" * 50)
                print(f"Job: {job.Label}")
                for issue in issues:
                    print(f"  ✗ {issue}")
                print("=" * 50)

        if not any_bad:
            print("✅ All jobs have correct postprocessor (marlin) and output file (jobs/<job_label>.gcode).")
