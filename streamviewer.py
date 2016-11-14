#!/usr/bin/env python3

import sys

from PyQt5.QtCore import Qt, QUrl, pyqtSignal
from PyQt5.QtWidgets import QApplication, QWidget, QGridLayout, QStackedLayout, QLabel
from PyQt5.QtGui import QMovie, QPalette
from PyQt5.QtMultimedia import QMediaPlayer, QMediaContent
from PyQt5.QtMultimediaWidgets import QVideoWidget


class StreamViewer(QWidget):
    streams_ready = pyqtSignal()

    def __init__(self, streams):
        self.streams = streams
        self.starting_player_indices = set(range(len(self.streams)))
        super().__init__()

        self.setAttribute(Qt.WA_QuitOnClose)
        self.setWindowFlags(Qt.FramelessWindowHint)

        palette = QPalette()
        palette.setColor(QPalette.Background, Qt.white)
        self.setPalette(palette)

        self.layout = QStackedLayout()
        self.setLayout(self.layout)

        label = QLabel()
        label.setAlignment(Qt.AlignCenter)
        self.loading_gif = QMovie("loading.gif")
        label.setMovie(self.loading_gif)
        self.layout.insertWidget(0, label)

        self.streams_ready.connect(self.showStartingScreen)
        self.players = []
        for stream in self.streams:
            player = QMediaPlayer()
            player.setMedia(QMediaContent(stream))
            player.mediaStatusChanged.connect(self.changedMediaStatus)
            player.play()
            self.players.append(player)

        grid_layout = QGridLayout()
        grid_layout.setSpacing(0)
        grid_layout.setContentsMargins(0, 0, 0, 0)
        self.grid_widget = QWidget()
        self.grid_widget.setLayout(grid_layout)

        i = 0
        self.grid_widgets = []
        for row in range(2):
            for column in range(2):
                video_widget = QVideoWidget()
                self.players[i].setVideoOutput(video_widget)
                self.grid_widgets.append(video_widget)
                grid_layout.addWidget(video_widget, row, column)
                i+=1
        self.layout.insertWidget(1, self.grid_widget)

        video_widget = QVideoWidget()
        self.players[4].setVideoOutput(video_widget)
        self.layout.insertWidget(2, video_widget)

        self.loading_gif.start()

    def showStartingScreen(self):
        self.showStreamGrid()
        self.loading_gif.stop()
        for player in self.players:
            player.mediaStatusChanged.disconnect(self.changedMediaStatus)

    def showStreamFullScreen(self, index):
        if index > 3:
            self.layout.setCurrentIndex(index - 2)
        else:
            self.grid_widgets[index].show()
            for i, widget in enumerate(self.grid_widgets):
                if i != index:
                    widget.hide()
            self.layout.setCurrentIndex(1)

    def showStreamGrid(self):
        for widget in self.grid_widgets:
            widget.show()
        self.layout.setCurrentIndex(1)

    def keyPressEvent(self, event):
        key = event.key()
        if key == Qt.Key_Escape:
            self.close()
        elif key in (Qt.Key_1, Qt.Key_2, Qt.Key_3, Qt.Key_4, Qt.Key_5):
            self.showStreamFullScreen(key - Qt.Key_1)
        elif key == (Qt.Key_0):
            self.showStreamGrid()

    def isStreamReady(self, player):
        return (player.mediaStatus() in (QMediaPlayer.BufferedMedia,
                                         QMediaPlayer.StalledMedia) or
                player.state() == QMediaPlayer.StoppedState)

    def changedMediaStatus(self, status):
        player = self.sender()
        index = self.players.index(player)
        if index in self.starting_player_indices and self.isStreamReady(player):
            self.starting_player_indices.remove(index)

        if not self.starting_player_indices:
            self.streams_ready.emit()


STREAM_IDS = [
    '04asu087t_1@409528',
    '074tzyqju_1@394719',
    '0i1p9ue46_1@409669',
    '04et6qkjl_1@409668',
    '0xorfu8d8_1@308855'
]

STREAM_URL_FMT = "http://streamamghdl-lh.akamaihd.net/i/{}/index_1_av-p.m3u8"

streams = [QUrl(STREAM_URL_FMT.format(id)) for id in STREAM_IDS]

app = QApplication(sys.argv)

viewer = StreamViewer(streams)
viewer.showFullScreen()

app.exec_()
