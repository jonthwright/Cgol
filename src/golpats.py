from PIL import Image
import sys
import os
import re

def extract_pattern(img: Image) -> list[list[int]]:
	pass

if __name__ == '__main__':
	img_path = sys.argv[1]
	filename = re.sub(r'\.(png|jpe?g)$', '', img_path).lower()

	if os.path.exists(img_path) and os.path.isfile(img_path):
		with open(filename, 'w+') as file:
			img = Image.open(img_path)
			pattern = extract_pattern(img)
			img.close()
   
			if pattern:
				file.write(f'{len(pattern[0])} {len(pattern)}')
				file.writelines(''.join(row) for row in pattern)

				print(f'Pattern >> {filename} << has been extracted...')
			else:
				raise TypeError(f'Pattern is of type + {type(pattern)}')