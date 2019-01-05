#ifndef GUARD_DY_DYMACROSETTINGS_H
#define GUARD_DY_DYMACROSETTINGS_H

/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/// @macro MDY_FLAG_MODE_POPULATE_COMPRESSED_DATAFILE
/// @brief If this flag is set, neither initialization a project nor running game,
/// but just bind every resources into compressed files, running procedure monitoring window.
///
/// When run project with this, any specified file name is exist, procedure will not take off.
/// - Data###.dydat (^Data(\t){3}.dydat$)
/// 
/// Compressed .dydat file will be detected by application,
/// when MDY_FLAG_LOAD_COMPRESSED_DATAFILE is set.
///
/// ** SEQUENCE... **
/// 1. define MDY_FLAG_MODE_POPULATE_COMPRESSED_DATAFILE.
/// 2. M_PATH_PLAIN_PATH_OF_SETTING_JSON string literal must specify the path of "Setting.json"
/// 2. build project and run program. Mode chagned to compression mode, wait until procedure finished.
/// 3. undefine MDY_FLAG_MODE_POPULATE_COMPRESSED_DATAFILE
/// 4. define MDY_FLAG_LOAD_COMPRESSED_DATAFILE, so compressed `Data###.dydat` must be loaded instead of plain json and resources. 
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//#define MDY_FLAG_MODE_POPULATE_COMPRESSED_DATAFILE 

/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/// @macro M_PATH_PLAIN_PATH_OF_SETTING_JSON
/// @brief 
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define M_PATH_PLAIN_PATH_OF_SETTING_JSON "./Project/Setting.json"

/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/// @macro MDY_FLAG_LOAD_COMPRESSED_DATAFILE
/// @brief
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define MDY_FLAG_LOAD_COMPRESSED_DATAFILE

#endif /// GUARD_DY_DYMACROSETTINGS_H
