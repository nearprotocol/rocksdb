#include "rocksdb/db.h"
#include "rocksdb/utilities/backupable_db.h"

using namespace rocksdb;

int main(int argc, char* argv[]) {
  assert(argc == 3);
  BackupEngineReadOnly* backup_engine;
  Status s = BackupEngineReadOnly::Open(
      Env::Default(), BackupableDBOptions(argv[1]), &backup_engine);
  assert(s.ok());
  backup_engine->RestoreDBFromLatestBackup(argv[2], argv[2]);
  delete backup_engine;
}