import numpy as np
from PIL import Image, ImageDraw, ImageFont
from typing import Tuple


def saveImgPPM3(path: str, img: np.ndarray):
    with open(path, "w+") as f:
        f.write("P3\n{} {}\n255\n".format(img.shape[0], img.shape[1]))
        temp = [
            "{} {} {}\n".format(*elem)
            for elem in img.reshape((img.shape[0] * img.shape[1], 3))
        ]
        f.writelines(temp)


def createImageWithText(
    path: str,
    width: int,
    height: int,
    font_size: int,
    font_color: Tuple[int, int, int],
    background_color: Tuple[int, int, int],
    unicode_text: str,
):
    unicode_font = ImageFont.truetype("DejaVuSans.ttf", font_size)

    img = Image.new("RGB", (width, height), background_color)
    draw = ImageDraw.Draw(img)
    w, h = draw.textsize(unicode_text, font=unicode_font)
    draw.text(
        ((width - w) / 2, (height - h) / 2),
        unicode_text,
        font=unicode_font,
        fill=font_color,
    )

    saveImgPPM3(path, np.array(img))


if __name__ == "__main__":
    createImageWithText(
        "textures/img.ppm", 200, 100, 48, (0, 0, 0), (244, 164, 96), "δ"
    )
