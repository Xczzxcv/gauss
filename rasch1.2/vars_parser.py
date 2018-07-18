import textract
import re



f = open('vars.txt', 'w')

#1) A(1, 4), B(3, -5), C(3, 2)
pattern = '\d+\) A\([-]?\d+, [-]?\d+\), B\([-]?\d+, [-]?\d+\), C\([-]?\d+, [-]?\d+\)'
num_pattern = '[-]?\d+'
list_strs_to_write = []

text = textract.process('rv2.pdf')
matches = re.findall(pattern, str(text))
for match in matches:
	list_strs_to_write.append(' '.join(re.findall(num_pattern, match)[1:]) + '\n')

f.write(str(len(matches)) + '\n')
f.write(''.join(list_strs_to_write))
f.close()