
//////////////////////////////////////////////////////////////////////
// Triangulation Group {

void PanelScan::_on_laserTriangulatorStarted() {

  MainWindow* mainWin = getApp()->getMainWindow();
  mainWin->setChangesEnabled(false);

  triangulationRunButton->setText("STOP");

  int count = laserTriangulator->count();
  scanningProgress->setMaximum(count);
  scanningProgress->setValue(0);

  // get the scene graph from the app 3D view
  SceneGraph* pWrl    = mainWin->getSceneGraph();
  if(pWrl!=nullptr) {
    pWrl->clear();

    // TODO Wed Apr 21 16:48:00 2021
    // - add scanner geometry to scene graph
    //   or at least the turntable

    // add IndexedfaceSet node named POINTS
    Shape* s = new Shape();
    pWrl->addChild(s);
    s->setName("POINTS");

    Appearance* a = new Appearance();
    s->setAppearance(a);

    Material* m = new Material();
    a->setMaterial(m);

    Color c(0.9f,0.9f,0.9f);
    m->setDiffuseColor(c);

    IndexedFaceSet* ifs = new IndexedFaceSet();
    s->setGeometry(ifs);
  } else /* if(pWrl==nullptr) */ {
    // TODO Wed Apr 21 16:22:00 2021
    // ERROR !!!
  }

  mainWin->refresh();
  // calibrateTurntableButton->setEnabled(false);
}

//////////////////////////////////////////////////////////////////////
void PanelScan::_on_laserTriangulatorProgress() {

  int index = laserTriangulator->index();
  scanningProgress->setValue(index);

  // TODO Wed Apr 21 16:14:27 2021
  // - instead of replacing all the POINTS after each frame
  //   incrementally add new points

  // get triangulated 3d points from laserTriangulator
  QVector<Vector3d>& worldCoord = laserTriangulator->worldCoord();
  int nPoints = worldCoord.size();
  if(nPoints>0) {

    // get the scene graph from the app 3D view
    MainWindow* mainWin = getApp()->getMainWindow();
    SceneGraph* pWrl    = mainWin->getSceneGraph();
    if(pWrl==nullptr) {
      // TODO Wed Apr 21 16:22:00 2021
      // ERROR !!!
    }

    Shape* shape = dynamic_cast<Shape*>(pWrl->find("POINTS"));
    if(shape==nullptr) {
      // TODO Wed Apr 21 16:20:29 2021
      // add a new Shape node named "POINTS" to the scene graph
    }

    Node* geometry = shape->getGeometry();
    IndexedFaceSet* ifs = dynamic_cast<IndexedFaceSet*>(geometry);
    if(ifs==nullptr) {
      if(geometry!=nullptr) delete geometry;
      ifs = new IndexedFaceSet();
      shape->setGeometry(ifs);
    }

    if(ifs!=(IndexedFaceSet*)0) {
      // remove all the properties for now
      ifs->getColor().clear();
      ifs->getNormal().clear();
      ifs->getColorIndex().clear();
      ifs->getNormalIndex().clear();
      ifs->getCoordIndex().clear();

      std::vector<float>& coord  = ifs->getCoord();
      for(int i=0;i<nPoints;i++) {
        coord.push_back(worldCoord[i](0));
        coord.push_back(worldCoord[i](1));
        coord.push_back(worldCoord[i](2));
      }
    }
    mainWin->refresh();
  }
}

//////////////////////////////////////////////////////////////////////
void PanelScan::_on_laserTriangulatorFinished() {

  triangulationRunButton->setText("RUN");

  auto mainWin = getApp()->getMainWindow();
  mainWin->setChangesEnabled(true);

  scanningProgress->setValue(0);
  triangulationRunButton->setChecked(false);
}
