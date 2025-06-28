package ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.estrategiasFuenteEstatica;

import ar.edu.utn.frba.dds.fuenteestatica.models.domain.Hecho;

import java.util.List;

public interface EstrategiaImportacionEstatica {

  List<Hecho> leerArchivo(String rutaFuente);
}
