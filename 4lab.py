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

class CanvasWidget(QWidget):
    def __init__(self):
        super().__init__()
        self.setFocusPolicy(Qt.StrongFocus)
        self.setMinimumSize(700, 450)
        self.setStyleSheet("background-color: white;")

        self.storage = MyStorage(200)
        self.current_shape_name = "rectangle"
        self.work_margin = 12

    def get_work_bounds(self):
        """Возвращение границы рабочей области"""
        left = self.work_margin
        top = self.work_margin
        right = self.width() - self.work_margin
        bottom = self.height() - self.work_margin
        return left, top, right, bottom

    def set_current_shape(self, shape_name):
        self.current_shape_name = shape_name

    def get_selected_shapes(self):
        return self.storage.get_selected_shapes()

    def set_color_for_selected(self, color):
        for shape in self.storage.get_selected_shapes():
            shape.set_color(color)
        self.update()

    def create_shape_by_name(self, x, y):
        """Создание фигуры по имени"""
        width = 140
        height = 90
        left, top, right, bottom = self.get_work_bounds()

        if self.current_shape_name == "circle":
            size = min(width, height) 
            x = max(left, min(int(x) - size // 2, right - size))
            y = max(top, min(int(y) - size // 2, bottom - size))
            return CircleShape(x, y, size, size, QColor("#816FD3"))
        else:
            x = max(left, min(int(x) - width // 2, right - width))
            y = max(top, min(int(y) - height // 2, bottom - height))
            
            if self.current_shape_name == "rectangle":
                return RectangleShape(x, y, width, height, QColor("#7ADFBA"))
            elif self.current_shape_name == "ellipse":
                return EllipseShape(x, y, width, height, QColor("#FFD3B6"))
            elif self.current_shape_name == "triangle":
                return TriangleShape(x, y, width, height, QColor("#FFAAA5"))

        return None

    def mousePressEvent(self, event):
        self.setFocus()

        if event.button() == Qt.LeftButton:
            point = event.pos()
            ctrl_pressed = bool(event.modifiers() & Qt.ControlModifier)

            if ctrl_pressed:
                shapes_under_cursor = self.storage.find_all_shapes_at(point.x(), point.y())
                
                if shapes_under_cursor:
                    for shape in shapes_under_cursor:
                        shape.set_selected(not shape.selected)
                else:
                    self.storage.clear_selection()
            else:
                self.storage.clear_selection()
                shape = self.storage.find_shape_at(point.x(), point.y())
                
                if shape is not None:
                    shape.set_selected(True)
                else:
                    new_shape = self.create_shape_by_name(point.x(), point.y())
                    if new_shape is not None:
                        self.storage.add(new_shape)

            self.update()

        super().mousePressEvent(event)

    def keyPressEvent(self, event):
        move_step = 10
        resize_step = 10 
        selected_shapes = self.storage.get_selected_shapes()
        shift_pressed = bool(event.modifiers() & Qt.ShiftModifier)

        left, top, right, bottom = self.get_work_bounds()

        if event.key() == Qt.Key_Delete:
            if selected_shapes:
                self.storage.remove_selected()
                self.update()
                return

        if not selected_shapes:
            super().keyPressEvent(event)
            return

        if shift_pressed:
            if event.key() == Qt.Key_Left or event.key() == Qt.Key_Right:
                delta = -resize_step if event.key() == Qt.Key_Left else resize_step
                for shape in selected_shapes:
                    shape.resize_uniform(delta, left, top, right, bottom)
                self.update()
                return
            elif event.key() == Qt.Key_Up or event.key() == Qt.Key_Down:
                delta = -resize_step if event.key() == Qt.Key_Up else resize_step
                for shape in selected_shapes:
                    shape.resize_uniform(delta, left, top, right, bottom)
                self.update()
                return
        else:
            if event.key() == Qt.Key_Left:
                for shape in selected_shapes:
                    shape.move_within_bounds(-move_step, 0, left, top, right, bottom)
                self.update()
                return
            elif event.key() == Qt.Key_Right:
                for shape in selected_shapes:
                    shape.move_within_bounds(move_step, 0, left, top, right, bottom)
                self.update()
                return
            elif event.key() == Qt.Key_Up:
                for shape in selected_shapes:
                    shape.move_within_bounds(0, -move_step, left, top, right, bottom)
                self.update()
                return
            elif event.key() == Qt.Key_Down:
                for shape in selected_shapes:
                    shape.move_within_bounds(0, move_step, left, top, right, bottom)
                self.update()
                return

        super().keyPressEvent(event)

    def resizeEvent(self, event):
        left, top, right, bottom = self.get_work_bounds()
        self.storage.fit_all_inside_bounds(left, top, right, bottom)
        self.update()
        super().resizeEvent(event)

    def paintEvent(self, event):
        super().paintEvent(event)

        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)

        left, top, right, bottom = self.get_work_bounds()
        work_rect = QRect(left, top, right - left, bottom - top)

        painter.setPen(QPen(Qt.black, 1))
        painter.setBrush(Qt.NoBrush)
        painter.drawRect(work_rect)

        self.storage.draw_all(painter)

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("ЛР4 - Визуальный редактор")
        self.resize(1000, 700)

        self._create_actions()
        self._create_menu()
        self._create_toolbar()
        self._create_central_widget()

    def _create_actions(self):
        self.rectangle_action = QAction("Прямоугольник", self)
        self.rectangle_action.triggered.connect(
            lambda: self.set_current_shape("rectangle")
        )

        self.ellipse_action = QAction("Эллипс", self)
        self.ellipse_action.triggered.connect(
            lambda: self.set_current_shape("ellipse")
        )

        self.circle_action = QAction("Круг", self)
        self.circle_action.triggered.connect(
            lambda: self.set_current_shape("circle")
        )

        self.triangle_action = QAction("Треугольник", self)
        self.triangle_action.triggered.connect(
            lambda: self.set_current_shape("triangle")
        )

        self.color_action = QAction("Цвет", self)
        self.color_action.setShortcut("C")
        self.color_action.triggered.connect(self.choose_color)

        self.exit_action = QAction("Выход", self)
        self.exit_action.triggered.connect(self.close)
        
        self.help_action = QAction("Справка", self)
        self.help_action.setShortcut("F1")
        self.help_action.triggered.connect(self.show_help)

    def _create_menu(self):
        menu_bar = self.menuBar()
        
        file_menu = menu_bar.addMenu("Файл")
        file_menu.addAction(self.exit_action)
        
        help_menu = menu_bar.addMenu("Справка")
        help_menu.addAction(self.help_action)

    def _create_toolbar(self):
        toolbar = QToolBar("Панель инструментов")
        toolbar.setMovable(False)
        self.addToolBar(toolbar)

        toolbar.addAction(self.rectangle_action)
        toolbar.addAction(self.ellipse_action)
        toolbar.addAction(self.circle_action)
        toolbar.addAction(self.triangle_action)
        toolbar.addAction(self.color_action)

    def _create_central_widget(self):
        central_widget = QWidget()
        layout = QVBoxLayout(central_widget)
        layout.setContentsMargins(12, 12, 12, 12)
        layout.setSpacing(8)

        self.info_label = QLabel("Выбранная фигура: Прямоугольник")
        self.info_label.setStyleSheet("font-size: 14px; padding: 6px; background-color: #f0f0f0; border: 1px solid #ccc;")

        self.canvas = CanvasWidget()

        layout.addWidget(self.info_label)
        layout.addWidget(self.canvas)

        self.setCentralWidget(central_widget)

    def show_help(self):
        """Показывает окно справки с управлением"""
        help_text = """
        <h3>Управление программой</h3>
        <ul>
            <li><b>Стрелки</b> - перемещение выделенной фигуры</li>
            <li><b>Shift + Стрелки</b> - равномерное изменение размера выделенной фигуры</li>
            <li><b>Delete</b> - удалить выделенную фигуру(ы)</li>
            <li><b>C</b> - изменить цвет выделенной фигуры(ы)</li>
            <li><b>Ctrl + клик</b> - множественный выбор фигур (включая пересекающиеся)</li>
            <li><b>Клик без Ctrl</b> - выделить одну фигуру или создать новую</li>
            <li><b>F1</b> - показать эту справку</li>
        </ul>
        """
        QMessageBox.information(self, "Справка", help_text)

    def set_current_shape(self, shape_name):
        self.canvas.set_current_shape(shape_name)

        shape_names = {
            "rectangle": "Прямоугольник",
            "ellipse": "Эллипс",
            "circle": "Круг",
            "triangle": "Треугольник",
        }

        self.info_label.setText(
            f"Выбранная фигура: {shape_names.get(shape_name, 'Неизвестно')}"
        )

    def choose_color(self):
        selected_shapes = self.canvas.get_selected_shapes()

        if not selected_shapes:
            QMessageBox.information(self, "Цвет", "Сначала выделите фигуру (кликните по ней)")
            return

        initial_color = selected_shapes[0].color
        color = QColorDialog.getColor(initial_color, self, "Выберите цвет")

        if color.isValid():
            self.canvas.set_color_for_selected(color)


def main():
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())


main()   