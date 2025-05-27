import domain.exceptions.ErrorImportacionCSVException;
import domain.fuentes.estrategiasFuenteEstatica.Archivo;
import domain.fuentes.FuenteEstatica;
import domain.fuentes.estrategiasFuenteEstatica.EstrategiaCSV;
import domain.Hecho;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertThrows;

public class TestImportarHechos {
  @Test
  @DisplayName("La fuente estatica puede importar hechos con la estrategia de archivo csv")
  public void importarHechos(){
    Archivo archivoNuevo = new Archivo("desastres_sanitarios_contaminacion_argentina.csv", 1024);
    EstrategiaCSV miEstrategia = new EstrategiaCSV(archivoNuevo ,20);
    FuenteEstatica miFuente = new FuenteEstatica("src/recursos/",miEstrategia);
    for (Hecho hechoImportado : miFuente.importarHechos()) {
      System.out.println(hechoImportado);
    }
  }
  @Test
  @DisplayName("La fuente estatica NO puede importar hechos con la estrategia de archivo csv porque el path esta mal")
  public void importarHechosFalla(){
    Archivo archivoNuevo = new Archivo("archivo.csv", 1024);
    EstrategiaCSV miEstrategia = new EstrategiaCSV(archivoNuevo, 10);
    FuenteEstatica miFuente = new FuenteEstatica("rutaInvalida",miEstrategia);
    assertThrows(ErrorImportacionCSVException.class, () -> {
      miFuente.importarHechos();
    });
  }
}
