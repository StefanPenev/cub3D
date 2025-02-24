import os
import subprocess

# ANSI color codes
GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
BLUE = "\033[94m"
RESET = "\033[0m"

def test_maps(executable, directory):
    # Get all .cub files in the directory
    map_files = [f for f in os.listdir(directory) if f.endswith(".cub")]

    if not map_files:
        print(YELLOW + "No .cub files found in the directory." + RESET)
        return

    passed_tests = []
    failed_tests = []
    passed_count = 0
    failed_count = 0

    for map_file in map_files:
        map_path = os.path.join(directory, map_file)
        print(BLUE + f"Testing map: {map_file}" + RESET)

        # Run cub3D
        try:
            result = subprocess.run([executable, map_path], capture_output=True, text=True)
            if result.returncode != 0:
                print(GREEN + f"[PASSED] {map_file} (Invalid map correctly rejected)" + RESET)
                passed_tests.append(map_file)
                passed_count += 1
            else:
                print(RED + f"[FAILED] {map_file} (Invalid map was accepted!)" + RESET)
                failed_tests.append(map_file)
                failed_count += 1
            print(YELLOW + "Output:" + RED + f" {result.stdout}" + RESET)
            if result.stderr:
                print(RED + "Errors:" + RESET, result.stderr)
        except Exception as e:
            print(RED + f"Error running {map_file}: {e}" + RESET)

        # Run Valgrind
        try:
            valgrind_result = subprocess.run([
                "valgrind", "--leak-check=full", "--error-exitcode=1", executable, map_path
            ], capture_output=True, text=True)

            # Combine stdout and stderr for a full report
            combined_output = valgrind_result.stdout + valgrind_result.stderr
            # Filter out duplicate error messages.
            filtered_output = "\n".join(
                line for line in combined_output.splitlines()
                if not line.startswith("Wrong input:") and "Missing floor or ceiling color." not in line
            )

            no_leaks_detected = "All heap blocks were freed -- no leaks are possible" in combined_output

            if no_leaks_detected:
                print(GREEN + f"[VALGRIND] Memory leaks not detected in {map_file}" + RESET)
            else:
                print(RED + f"[VALGRIND] Memory leaks detected in {map_file}" + RESET)

            print(YELLOW + "Valgrind Report:" + RESET)
            print(filtered_output)
        except Exception as e:
            print(RED + f"Error running Valgrind on {map_file}: {e}" + RESET)

        print("-" * 50)  # Separator for readability

    # Final summary
    print(BLUE + "========== TEST SUMMARY ==========" + RESET)
    print(GREEN + f"✅ Passed tests: {passed_count}" + RESET)
    if passed_tests:
        print(GREEN + "Passed: " + ", ".join(passed_tests) + RESET)
    print(RED + f"❌ Failed tests: {failed_count}" + RESET)
    if failed_tests:
        print(RED + "Failed: " + ", ".join(failed_tests) + RESET)
    print(BLUE + "==================================" + RESET)

if __name__ == "__main__":
    test_maps("./cub3D_bonus", "maps/not_valid")
