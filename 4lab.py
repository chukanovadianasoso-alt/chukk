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


class RectangleShape(Shape):
    def draw(self, painter):
        if self.selected:
            painter.setPen(QPen(QColor("blue"), 3))
        else:
            painter.setPen(QPen(Qt.black, 2))
        painter.setBrush(QBrush(self.color))
        painter.drawRect(self.get_rect())


class EllipseShape(Shape):
    def draw(self, painter):
        if self.selected:
            painter.setPen(QPen(QColor("blue"), 3))
        else:
            painter.setPen(QPen(Qt.black, 2))
        painter.setBrush(QBrush(self.color))
        painter.drawEllipse(self.get_rect())


class CircleShape(Shape):
    def draw(self, painter):
        if self.selected:
            painter.setPen(QPen(QColor("blue"), 3))
        else:
            painter.setPen(QPen(Qt.black, 2))
        painter.setBrush(QBrush(self.color))
        size = min(self.width, self.height)
        rect = QRect(self.x, self.y, size, size)
        painter.drawEllipse(rect)


class TriangleShape(Shape):
    def draw(self, painter):
        if self.selected:
            painter.setPen(QPen(QColor("blue"), 3))
        else:
            painter.setPen(QPen(Qt.black, 2))
        painter.setBrush(QBrush(self.color))

        points = QPolygon(
            [
                QPoint(self.x + self.width // 2, self.y),
                QPoint(self.x, self.y + self.height),
                QPoint(self.x + self.width, self.y + self.height),
            ]
        )

        painter.drawPolygon(points)

class MyStorage:
    """Контейнер"""
    def __init__(self, capacity: int = 100):
        self.__items = [None] * capacity
        self.__count = 0

    def get_count(self):
        return self.__count

    def set_object(self, index: int, obj: Shape):
        if index < 0 or index >= len(self.__items):
            raise IndexError("Index out of bounds")
        if obj is None:
            raise ValueError("Cannot store None")
        self.__items[index] = obj
        if index >= self.__count:
            self.__count = index + 1

    def get_object(self, index: int):
        if index < 0 or index >= self.__count:
            raise IndexError("Index out of bounds")
        return self.__items[index]

    def add(self, obj: Shape):
        if self.__count >= len(self.__items):
            raise OverflowError("Storage is full")
        self.__items[self.__count] = obj
        self.__count += 1

    def remove_at(self, index: int):
        if index < 0 or index >= self.__count:
            raise IndexError("Index out of bounds")
        for i in range(index, self.__count - 1):
            self.__items[i] = self.__items[i + 1]
        self.__items[self.__count - 1] = None
        self.__count -= 1
    
    def remove_selected(self):
        i = 0
        while i < self.__count:
            shape = self.__items[i]
            if shape and shape.selected:
                self.remove_at(i)
            else:
                i += 1
    
    def clear_selection(self):
        for i in range(self.__count):
            if self.__items[i]:
                self.__items[i].set_selected(False)
    
    def get_selected_shapes(self):
        return [self.__items[i] for i in range(self.__count) if self.__items[i] and self.__items[i].selected]
    
    def find_shape_at(self, x, y):
        for i in range(self.__count - 1, -1, -1):
            shape = self.__items[i]
            if shape and shape.contains_point(x, y):
                return shape
        return None

    def find_all_shapes_at(self, x, y):
        shapes = []
        for i in range(self.__count - 1, -1, -1):
            shape = self.__items[i]
            if shape and shape.contains_point(x, y):
                shapes.append(shape)
        return shapes
    
    def draw_all(self, painter):
        for i in range(self.__count):
            if self.__items[i]:
                self.__items[i].draw(painter)
    
    def fit_all_inside_bounds(self, left, top, right, bottom):
        for i in range(self.__count):
            if self.__items[i]:
                self.__items[i].fit_inside_bounds(left, top, right, bottom)