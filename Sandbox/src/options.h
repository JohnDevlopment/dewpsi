#ifndef OPTIONS_H
#define OPTIONS_H

struct SandboxData;

int parseArguments(int, const char*[], SandboxData* data);
bool readIniFile(const char* file, SandboxData* data);

#endif /* OPTIONS_H */
