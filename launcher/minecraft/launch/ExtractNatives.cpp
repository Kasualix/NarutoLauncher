/* Copyright 2013-2021 MultiMC Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ExtractNatives.h"
#include <launch/LaunchTask.h>
#include <minecraft/MinecraftInstance.h>

#include <QDir>
#include "FileSystem.h"
#include "archive/ArchiveReader.h"
#include "archive/ArchiveWriter.h"

#ifdef major
#undef major
#endif
#ifdef minor
#undef minor
#endif

static Result<> unzipNatives(QString source, QString targetFolder)
{
    MMCZip::ArchiveReader zip(source);
    QDir directory(targetFolder);
    if (directory.exists()) {
        directory.setPath(directory.canonicalPath());
    }

    auto extPtr = MMCZip::ArchiveWriter::createDiskWriter();
    auto ext = extPtr.get();

    return zip.parse([directory, ext](MMCZip::ArchiveReader::File* f) {
        QString name = f->filename();
        QString absFilePath = directory.absoluteFilePath(name);
        return f->writeFile(ext, absFilePath, directory);
    });
}

void ExtractNatives::executeTask()
{
    auto instance = m_parent->instance();
    auto toExtract = instance->getNativeJars();
    if (toExtract.isEmpty()) {
        emitSucceeded();
        return;
    }

    auto outputPath = instance->getNativePath();
    FS::ensureFolderPathExists(outputPath);
    for (const auto& source : toExtract) {
        if (const auto result = unzipNatives(source, outputPath); !result) {
            const char* reason = QT_TR_NOOP("Couldn't extract native jar '%1' to destination '%2': %3");
            emit logLine(QString(reason).arg(source, outputPath, result.error()), MessageLevel::Fatal);
            emitFailed(tr(reason).arg(source, outputPath, result.error()));
            return;
        }
    }
    emitSucceeded();
}

void ExtractNatives::finalize()
{
    auto instance = m_parent->instance();
    QString target_dir = FS::PathCombine(instance->instanceRoot(), "natives/");
    QDir dir(target_dir);
    dir.removeRecursively();
}
