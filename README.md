Небольшой набор программ (исходники на c++, соберутся в qt creator) для преобразования svg файлов (изображения с сегментацией для компьютерного зрения) в JSON файлы. Координаты точек преобразуются в абсолютные.

A small set of programs (sources on C ++, gather in QT Creator) for converting SVG files (images with segmentation for computer vision) into json files. The coordinates of the points are converted into absolute.

https://github.com/tsvigo/izvlechenie_putey_SVG_2/tree/master
----------------------------------------------------------
https://github.com/tsvigo/json_generation/tree/master
----------------------------------------------------------
https://github.com/tsvigo/svg_to_json_7/tree/master
---------------------------------------------------------
1. izvlechenie_putey_SVG_2 - извлекает из общего svg файла только пути то есть наборы координат масок сегментации. Затем записывает их в отдельные папки с отдельными svg.
2. svg_to_json_7 - извлекает и преобразует из отдельных svg файлов x и y координаты
3. json_generation - генерирует JSON файл
4.  Папки с отдельными svg файлами создаются в папке с исполняемой программой.
