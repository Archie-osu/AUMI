#pragma once

static void* pfnoYYError = NULL;

void Hook_YYError(const char* pFormat, ...);

void* GetYYErrorAddress();