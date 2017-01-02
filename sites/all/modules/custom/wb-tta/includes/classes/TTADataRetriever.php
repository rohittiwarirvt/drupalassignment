<?php

class TTADataRetriever {

  protected $TTAFileDirName, $agency;

  public function extractFiles($file) {
    $this->unpackZipToDir($file, $this->TTAFileDir());
  }

  public function TTAFileDir() {
    $time = time();

    $this->TTAFileDirName = "sites/default/files/tta_scratch/{$this->agency}/$time/";
    return $this->TTAFileDirName;
  }

  public function __construct($agency) {
    $this->agency = $agency;
  }

  private function unpackZipToDir($file, $destination_path) {
    $zip = new ZipArchive();
    $new_filepath = $destination_path . $this->agency;
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
    $dirs = array_filter(glob($destination_path .'/*'), 'is_dir');
    rename($dirs[0], $new_filepath);
  }

}
