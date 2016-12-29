<?php


class TTADataImporter {


  public function importTTAData(){
    //$txn = db_transaction();
    $this->setBaseDir($baseDir);
    //$this->deleteExistingGTFSData();
    //$this->importGTFSDataInDirectory($this->getRegularGTFSDir());
    //$this->importCSV($agenciesFilepath, "{$this->agencyName}_gtfs_agencies", $this->getAgenciesColumnMap());
  }

 /**
     * Helper function for inserting/upserting data from a CSV into a database table.
     * @param $filepath string path to the CSV on disk
     * @param $tableName string table to insert/upsert into
     * @param $columnsMap array mapping of csv columns to database columns
     * @param null $upsertKey string (optional) if specified, upserts will be performed instead of inserts. The specified db column name is used to determine whether to insert or update
     * @throws GTFSDataProcessorException
     */
    private function importCSV($filepath) {

    // Format columns map
    $fp = fopen($filepath, "r");
    if (!$fp) {
      // If there is no header information found, then there is no file data to import
            return array();
        }
        // Start processing header positions on the real import
        $headers = fgetcsv($fp, 0, ",");
        // Flip the array to get the numeric indexes as values and the values as keys
        $headers = array_flip($headers);
    fclose($fp);
    // Create a new columns map
    $newcolumnsMap = array();
    // Flip the columns map to get the file columns as the keys and the file data as the value
    $flippedColumnMap = array_flip($columnsMap);
    // Process the new headers map based on the first row of the file
    foreach ($headers as $key => $header) {
      // Search for the currently processed header on the Array values of the header mapping
      if (array_key_exists($key, $flippedColumnMap)) {
        $newcolumnsMap[$flippedColumnMap[$key]] = $header;
      } else if (array_key_exists(substr($key, 3), $flippedColumnMap)) {
        // Check if the value is not being recognized due to csv prefix on variable
        $newcolumnsMap[$flippedColumnMap[substr($key, 3)]] = $header;
      }
    }
    $csvData = array();
    if ($tablename == "{$this->agencyName}_gtfs_timepoints") {
      $csvData = $this->extractCSVDataForTimepoints($filepath, $newcolumnsMap);
    } else {
      $csvData = $this->extractCSVData($filepath, $newcolumnsMap);
    }
    if (!$upsertKey) {
            $query = db_insert($tableName)->fields(array_keys($newcolumnsMap));
        }

    $this->processLineNumber = 1;
        foreach ($csvData as $rowData) {
          if ($upsertKey) {
                db_merge($tableName)->key(array($upsertKey => $rowData[$upsertKey]))->fields($rowData)->execute();
            } else {
                $query->values($rowData);
            }
          $this->processLineNumber++;
        }

        if (!$upsertKey) {
            $query->execute();
        }
    }

    private function extractCSVData($filepath, $columnsMap) {
        $csvData = array();

        $fp = fopen($filepath, "r");

        if (!$fp) {
            return array();
            //$this->generateException("Could not open CSV file: $filepath");
        }

    fgetcsv($fp, 0, ","); # Skip headers

        while (($row = fgetcsv($fp, 0, ",")) !== FALSE) {
            $csvData[] = $this->extractDataFromRow($row, $columnsMap);
        }

        fclose($fp);

        return $csvData;
    }

    private function extractCSVDataForTimepoints($filepath, $columnsMap) {
        $csvData = array();

        $fp = fopen($filepath, "r");

        if (!$fp) {
            return array();
            //$this->generateException("Could not open CSV file: $filepath");
        }

    fgetcsv($fp, 0, ","); # Skip headers

        while (($row = fgetcsv($fp, 0, ",")) !== FALSE) {
          $newcsvData = $this->extractDataFromRow($row, $columnsMap);
            $csvData[] = $newcsvData;
          // Update the stop and set it to timepoint
          $sql = "UPDATE {$this->agencyName}_gtfs_stop_times st
                      JOIN {$this->agencyName}_gtfs_trips t ON (st.trip_id = t.trip_id)
                       SET st.is_time_point = 1
                     WHERE st.stop_id = :stop_id AND t.route_id = :route_id AND t.service_id = (:service_id)";
            $bindVars = array(':stop_ids' => $newcsvData['stop_id'], ':route_id' => $newcsvData['route'], ':service_id' => $newcsvData['service_id']);
            db_query($sql, $bindVars);
        }

        fclose($fp);

        return $csvData;
    }

    /**
     * $columnsMap format: array( <column_name> => <csv_column_number>, ...)
     * @param $row array csv data
     * @param $columnsMap array maps the database column to the column in a csv file
     * @return array returns an array of format array( <column_name> => <column_value>, ...)
     */
    private function extractDataFromRow($row, $columnsMap) {
        $rowData = array();

        foreach ($columnsMap as $columnName => $rowIndex) {
            $rowData[$columnName] = $row[$rowIndex];
        }

        return $rowData;
    }

    private function importTimePointsDataInDirectory($dataDir) {
        try {
            $searchPath = $this->getTimePointsDir()."/*.csv";
            $timePointFiles = glob($searchPath);

            if ($timePointFiles === false || count($timePointFiles) == 0) {
                //$this->generateException("Could not locate TTA Time Points data matching: $searchPath");
            }

            foreach ($timePointFiles as $timePointFile) {
                $routeId = null;
                $stops = array();
                $serviceIds = array();
                $fp = fopen($timePointFile, "r");
                while (!feof($fp)) {
                    $lineVals = fgetcsv($fp);
                    if (!$lineVals) {
                        continue;
                    }

                    if ($lineVals[0] == 'SERVICE') {
                        $serviceIds = explode(",", $lineVals[1]);
                    } elseif ($lineVals[0] == 'LINE') {
                        $routeId = $lineVals[1];
                    } elseif ($lineVals[0] == 'STOPID') {
                        foreach (array_slice($lineVals, 1) as $stopId) {
                            if ($stopId) {
                                $stops[] = $stopId;
                            }
                        }
                    }
                }
                fclose($fp);

                if (!$routeId) {
                    $this->generateException("Could not determine route for time point CSV: $timePointFile");
                }

                if (count($serviceIds) == 0) {
                    $this->generateException("Could not determine service ids for time point CSV: $timePointFile");
                }

                if (count($stops) == 0) {
                    $this->generateException("Could not determine time points for time point CSV: $timePointFile");
                }

                $sql = "UPDATE {$this->agencyName}_gtfs_stop_times st
                          JOIN {$this->agencyName}_gtfs_trips t ON (st.trip_id = t.trip_id)
                           SET st.is_time_point = 1
                         WHERE st.stop_id IN (:stop_ids) AND t.route_id = :route_id AND t.service_id IN (:service_ids)";
                $bindVars = array(':stop_ids' => $stops, ':route_id' => $routeId, ':service_ids' => $serviceIds);
                db_query($sql, $bindVars);
            }

        } catch (Exception $ex) {
            $this->generateException("Error reading GIS data file: ".$ex->getMessage());
        }
    }
}
