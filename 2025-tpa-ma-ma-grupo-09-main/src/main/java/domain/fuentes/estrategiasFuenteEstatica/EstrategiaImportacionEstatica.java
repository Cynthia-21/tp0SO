package domain.fuentes.estrategiasFuenteEstatica;

import domain.Hecho;
import java.util.List;

public interface EstrategiaImportacionEstatica {

  List<Hecho> leerArchivo(String rutaFuente);
}
