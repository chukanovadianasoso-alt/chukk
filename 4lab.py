import sys
from math import sqrt

from PyQt5.QtCore import QPoint, QRect, Qt
from PyQt5.QtGui import QPainter, QPen, QBrush, QColor, QPolygon
from PyQt5.QtWidgets import (
    QApplication,
    QColorDialog,
    QLabel,
    QMainWindow,
    QToolBar,
    QVBoxLayout,
    QWidget,
    QAction,
    QMessageBox
)


class Shape:
    """Базовый класс для всех фигур"""
    def __init__(self, x, y, width, height, color):
        self.x = int(x)
        self.y = int(y)
        self.width = int(width)
        self.height = int(height)
        self.color = color
        self.selected = False

    def get_rect(self):
        return QRect(self.x, self.y, self.width, self.height)

    def set_selected(self, selected):
        self.selected = selected

    def set_color(self, color):
        self.color = color

    def move_within_bounds(self, dx, dy, left, top, right, bottom):
        """Перемещение фигуры с проверкой границ"""
        new_x = self.x + int(dx)
        new_y = self.y + int(dy)

        new_x = max(left, min(new_x, right - self.width))
        new_y = max(top, min(new_y, bottom - self.height))

        self.x = new_x
        self.y = new_y

    def resize_uniform(self, delta, left, top, right, bottom):
        new_width = max(20, self.width + delta)
        new_height = max(20, self.height + delta)
        
        max_width = max(20, right - self.x)
        max_height = max(20, bottom - self.y)
        
        max_allowed = min(max_width, max_height)
        
        new_width = min(new_width, max_allowed)
        new_height = min(new_height, max_allowed)
        
        dx = (self.width - new_width) // 2
        dy = (self.height - new_height) // 2
        
        self.width = new_width
        self.height = new_height
        self.x += dx
        self.y += dy

        self.x = max(left, min(self.x, right - self.width))
        self.y = max(top, min(self.y, bottom - self.height))

    def fit_inside_bounds(self, left, top, right, bottom):
        """Подгонка фигуры под границы"""
        max_width = max(20, right - left)
        max_height = max(20, bottom - top)

        self.width = min(self.width, max_width)
        self.height = min(self.height, max_height)

        self.x = max(left, min(self.x, right - self.width))
        self.y = max(top, min(self.y, bottom - self.height))

    def contains_point(self, px, py):
        """Проверка, содержит ли фигура точку"""
        return self.get_rect().contains(int(px), int(py))

    def draw(self, painter):
        raise NotImplementedError

