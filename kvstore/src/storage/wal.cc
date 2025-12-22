#include "kvstore/src/storage/wal.h"
#include <iostream>

WriteAheadLog::WriteAheadLog(const std::string &filename)
    : filename_(filename) {
  // Open the file in Binary mode + Append mode
  log_file_.open(filename, std::ios::binary | std::ios::app);
}

WriteAheadLog::~WriteAheadLog() {
  if (log_file_.is_open()) {
    log_file_.close();
  }
}

bool WriteAheadLog::ReadAll(
    std::function<void(const kvstore::LogEntry &)> visitor) {
  std::lock_guard<std::mutex> lock(mu_);

  // Flush current writer to ensure everything is on disk
  if (log_file_.is_open()) {
    log_file_.flush();
  }

  // Open file for reading
  std::ifstream input(filename_, std::ios::binary);
  if (!input.is_open()) {
    return false; // File might not exist yet, which is fine for new store
  }

  while (true) {
    uint32_t size;
    // Read the size of the next entry (4 bytes)
    if (!input.read(reinterpret_cast<char *>(&size), sizeof(size))) {
      break; // End of file or error
    }

    std::string buffer;
    buffer.resize(size);
    // Read the actual protobuf data
    if (!input.read(&buffer[0], size)) {
      break; // Should not happen if file is uncorrupted
    }

    kvstore::LogEntry entry;
    if (entry.ParseFromString(buffer)) {
      visitor(entry); // Callback to restore state
    }
  }

  return true;
}

bool WriteAheadLog::Append(const std::string &key, const std::string &value) {
  // Create protobuf object
  kvstore::LogEntry entry;
  entry.set_op(kvstore::LogEntry::PUT);
  entry.set_key(key);
  entry.set_value(value);
  // TODO: set timestamp to current time
  entry.set_timestamp(time(NULL));

  // Serialize object into simple string
  std::string serialized_data;
  if (!entry.SerializeToString(
          &serialized_data)) { // Takes LogEntry object and squashes into single
                               // raw string of bytes so that it can be written
                               // to file
    return false;              // Error with failed serialization
  }

  // Thread safety: mutex lock
  std::lock_guard<std::mutex> lock(mu_); // freezes file for one thread to write

  uint32_t size =
      serialized_data
          .size(); // gets size to tell reader how long object is later

  // gets mem address of size, pretends the entire thing is a array of chars,
  // writes exactly 4 bytes (aka size of uint32_t)
  log_file_.write(reinterpret_cast<const char *>(&size),
                  sizeof(size)); // writes size of object

  log_file_.write(serialized_data.data(),
                  size); // writes actual content of protobuf

  // flush to OS buffer
  log_file_.flush();

  return log_file_.good();
}