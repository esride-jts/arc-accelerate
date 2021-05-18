import os
import unittest

from accelerate import Dataset

class TestAccelerateDataset(unittest.TestCase):

    def test_read(self):
        in_table = '{0}/ACLED_Sample'.format(os.environ['accelerate_sample_gdb'])
        dataset = Dataset()
        dataset.read(in_table, ['*'], '1=1')
        dataframe = dataset.to_pandas()
        row_count = dataframe.count()[0]
        self.assertLess(0, row_count, 'The dataframe should not be empty!')



if __name__ == '__main__':
    unittest.main()