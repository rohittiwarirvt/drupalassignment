<?php

class TTADataRetriever {

  protected $TTAFileDirName;

  public function extractFiles($file) {
    $this->unpackZipToDir($file, $this->TTAFileDir());
  }

  public function TTAFileDir() {
    return $this->TTAFileDirName;
  }

  public function __construct() {
    $time = time();
    $this->TTAFileDirName = "sites/default/files/tta_scratch/$time/";
  }

  private function unpackZipToDir($file, $destination_path) {
    $zip = new ZipArchive();
    $new_filepath = $destination_path . basename($file);
    try {
      if ($zip->open($file)) {
        $zip->extractTo($destination_path);
        $zip->close();
      } else {
         drupal_set_message(t("Unable to open file "), 'error');
      }
    }catch(Exception $e) {
      throw new Excetion ("Failed to extract ZIP: $file due to $e");
    }

    rename($file, $new_filepath);
  }

}
