package ar.edu.utn.frba.dds.fuenteestatica.services;

import ar.edu.utn.frba.dds.fuenteestatica.models.domain.Hecho;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.FuenteEstatica;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.estrategiasFuenteEstatica.Archivo;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.estrategiasFuenteEstatica.EstrategiaCSV;
import ar.edu.utn.frba.dds.fuenteestatica.models.repositories.IFuenteEstaticaRepository;
import ar.edu.utn.frba.dds.fuenteestatica.models.repositories.IHechoRepository;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class SeederService {
  @Autowired
  IHechoRepository repositorioDeHechos;
  IFuenteEstaticaRepository repositorioDeFuentes;
  public void init(){
    Archivo archivoNuevo = new Archivo("desastres_sanitarios_contaminacion_argentina.csv", 1024);
    EstrategiaCSV miEstrategia = new EstrategiaCSV(archivoNuevo ,20);
    FuenteEstatica miFuente = new FuenteEstatica("src/recursos/",miEstrategia);
    repositorioDeFuentes.save(miFuente);

    for (Hecho hechoImportado : miFuente.importarHechos()) {
      repositorioDeHechos.save(hechoImportado);
    }
  }
}
