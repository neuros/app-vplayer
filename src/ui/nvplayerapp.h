#ifndef _NVPLAYERAPP_H_
#define _NVPLAYERAPP_H_
/*
 *  Copyright(C) 2007 Neuros Technology International LLC.
 *               <www.neurostechnology.com>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2 of the License.
 *
 *
 *  This program is distributed in the hope that, in addition to its
 *  original purpose to support Neuros hardware, it will be useful
 *  otherwise, but WITHOUT ANY WARRANTY; without even the implied
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ****************************************************************************
 *
 * Video Player main instance header.
 *
 * REVISION:
 *
 * 1) Initial creation. ----------------------------------- 2008-08-27 WX
 *
 */

#include <QObject>
#include <QStringList>
#include <QPointer>
#include "nvideoplaybacksettings.h"
#include "nvideoplayer.h"

class NVPlayerApp : public QObject
{
    Q_OBJECT

public:
    NVPlayerApp(const QStringList &arguments = QStringList(), QObject *parent = NULL);
    ~NVPlayerApp();

public Q_SLOTS:
    bool start();

private Q_SLOTS:
    void OnQuit();

private:
    QString playPath;
    QString subtitleFilePath;
    NVideoPlaybackSettings::PlayProportion playProportion;

    QPointer<NVideoPlayer> vplayer;
};

#endif // _NVPLAYERAPP_H_
