import dv_processing as dv
import sys

if len(sys.argv) != 3:
    print("Usage: python extract_event_data.py <input_file> <output_file>")
    sys.exit(1)

reader = dv.io.MonoCameraRecording(sys.argv[1])

events = []

first_timestamp = -1

while reader.isRunning():
    event_batch = reader.getNextEventBatch()

    if event_batch is None:
        break

    if first_timestamp == -1:
        first_timestamp = event_batch[0].timestamp()

    for event in event_batch:
        events.append((event.timestamp() - first_timestamp, event.x(), event.y()))

with open(sys.argv[2], 'w') as f:
    f.write(f"size_t event_count = {len(events)};\n")
    f.write("event_t event_buffer[] = {\n")

    for (timestamp, x, y) in events:
        f.write("\t{")
        f.write(f"{timestamp},{x},{y}")
        f.write("},\n")

    f.write("};\n")