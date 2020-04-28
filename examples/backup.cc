#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "rocksdb/db.h"
#include "rocksdb/utilities/backupable_db.h"
using namespace rocksdb;

int main(int argc, char* argv[]) {
  assert(argc == 4);
  Options options;
  options.create_if_missing = true;
  DB* db;
  std::vector<ColumnFamilyDescriptor> column_families;
  // have to open default column family
  column_families.push_back(
      ColumnFamilyDescriptor(kDefaultColumnFamilyName, ColumnFamilyOptions()));
  for (int i = 0; i < atoi(argv[1]); i++) {
    char buf[10];
    sprintf(buf, "col%d", i);
    column_families.push_back(
        ColumnFamilyDescriptor(buf, ColumnFamilyOptions()));
  }

  std::vector<ColumnFamilyHandle*> handles;
  Status s =
      DB::OpenForReadOnly(DBOptions(), argv[2], column_families, &handles, &db);
  std::cout << s.ToString() << std::endl;
  assert(s.ok());

  BackupEngine* backup_engine;
  s = BackupEngine::Open(Env::Default(), BackupableDBOptions(argv[3]),
                         &backup_engine);
  assert(s.ok());
  s = backup_engine->CreateNewBackup(db);
  assert(s.ok());

  std::vector<BackupInfo> backup_info;
  backup_engine->GetBackupInfo(&backup_info);

  // you can get IDs from backup_info if there are more than two
  s = backup_engine->VerifyBackup(1 /* ID */);
  assert(s.ok());

  delete db;
  delete backup_engine;
}