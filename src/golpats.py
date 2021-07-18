from PIL import Image
import sys
import os
import re
import argparse


def extract_pattern(img: Image) -> list[list[str]]:
	width, height = img.size
	pattern = []

	for r in range(2, height, 4):
		pattern.append([])
		for c in range(2, width, 4):
			pix = img.getpixel((c, r))
			if pix == (0, 0, 0):
				pattern[-1].append('*')
			elif pix == (255, 255, 255):
				pattern[-1].append('_')

	return pattern

def write_pattern(img_path: str) -> None:
	filename = re.sub(r'\.(png|jpe?g)$', '', img_path).lower()

	if os.path.exists(img_path) and os.path.isfile(img_path):
		with open(filename, 'w+') as file:
			img = Image.open(img_path)
			rgb_img = img.convert('RGB')
			pattern = extract_pattern(rgb_img)
			img.close()

			if pattern:
				file.writelines((size := f'{len(pattern[0])} {len(pattern)}\n'))

				print(size[:-1])

				for row in pattern:
					row_str = ''.join(row)
					print(row_str)
					file.writelines(row_str)
					file.writelines('\n')

				print(f'Pattern >> {filename} << has been extracted...')
			else:
				raise TypeError(f'pattern is of type: {type(pattern)}')
	

if __name__ == '__main__':
	try:
		img_path = sys.argv[1]
		write_pattern(img_path)
	except TypeError as type_error:
		print(f'Handling pattern-extraction error: {type_error}')
	