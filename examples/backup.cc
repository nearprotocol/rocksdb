#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "rocksdb/db.h"
#include "rocksdb/utilities/backupable_db.h"
using namespace rocksdb;

int main(int argc, char* argv[]) {
  assert(argc == 3);
  Options options;
  DB* db;

  std::vector<std::string> column_family_names;
  std::vector<ColumnFamilyDescriptor> column_families;
  DB::ListColumnFamilies(options, argv[1], &column_family_names);

  // have to open default column family
  // column_families.push_back(
  //     ColumnFamilyDescriptor(kDefaultColumnFamilyName,
  //     ColumnFamilyOptions()));
  for (auto name : column_family_names) {
    column_families.push_back(
        ColumnFamilyDescriptor(name, ColumnFamilyOptions()));
  }

  std::vector<ColumnFamilyHandle*> handles;
  Status s =
      DB::OpenForReadOnly(DBOptions(), argv[1], column_families, &handles, &db);
  assert(s.ok());

  BackupEngine* backup_engine;
  s = BackupEngine::Open(Env::Default(), BackupableDBOptions(argv[2]),
                         &backup_engine);
  assert(s.ok());
  s = backup_engine->CreateNewBackup(db);
  assert(s.ok());

  std::vector<BackupInfo> backup_info;
  backup_engine->GetBackupInfo(&backup_info);

  // you can get IDs from backup_info if there are more than two
  s = backup_engine->VerifyBackup(backup_info.back().backup_id);
  assert(s.ok());

  delete db;
  delete backup_engine;
}