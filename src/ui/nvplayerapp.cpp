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
 * Video Player main instance source.
 *
 * REVISION:
 *
 * 1) Initial creation. ----------------------------------- 2008-08-27 WX
 *
 */

//#define QT_NO_DEBUG_OUTPUT
#include <QDebug>
#include "nvplayerapp.h"

#define SYSTEM_CONFIG     "/media/ext/data/sysconfig.ini"

NVPlayerApp::NVPlayerApp(const QStringList &arguments, QObject *parent)
: QObject(parent)
{
    /* load the playback proportion settings */
    QSettings settings(SYSTEM_CONFIG, QSettings::IniFormat);
    playProportion = (NVideoPlaybackSettings::PlayProportion)settings.value(PLAY_PROPORTION, DEFAULT_PLAY_PROPORTION).toInt();

    /* Parse the arguments */
    QStringListIterator argIt(arguments);
    while (argIt.hasNext())
    {
        QString arg = argIt.next();
        if (arg.startsWith("--path=")) // --path=/usb/video/...
            playPath = arg.remove(0, arg.indexOf('=') + 1);
        else if (arg == "-p" && argIt.hasNext()) // -p /usb/video/...
            playPath = argIt.next();
        else if (arg.startsWith("--subtitles=")) // --subtitles=/usb/subtitles/...
            subtitleFilePath = arg.remove(0, arg.indexOf('=') + 1);
        else if (arg == "-st" && argIt.hasNext()) // -st /usb/subtitles/...
            subtitleFilePath = argIt.next();
        else if (arg.startsWith("--proportions=")) // -proportions=normal/widescreen
            playProportion = (arg.remove(0, arg.indexOf('=') + 1) == "normal" ? NVideoPlaybackSettings::PPNormal : NVideoPlaybackSettings::PPWideScreen);
        else if (arg == "-ppt" && argIt.hasNext()) // -ppt normal/widescreen
            playProportion = (argIt.next() == "normal" ? NVideoPlaybackSettings::PPNormal : NVideoPlaybackSettings::PPWideScreen);
    }
}

NVPlayerApp::~NVPlayerApp()
{
    if (!vplayer.isNull())
        delete vplayer;
}

bool NVPlayerApp::start()
{
    qDebug( ) << playPath << subtitleFilePath << playProportion;

    if (vplayer.isNull())
    {
        vplayer = new NVideoPlayer(0);
        vplayer->setAttribute(Qt::WA_DeleteOnClose, true);
    }

    bool ok = vplayer->play();

    if (ok)
        vplayer->showMaximized();
    else
        vplayer->close();

    return ok;
}

void NVPlayerApp::OnQuit()
{
    Q_FOREACH (QWidget *widget, QApplication::topLevelWidgets())
        widget->close();
}
