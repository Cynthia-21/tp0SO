package ar.edu.utn.frba.dds.fuenteestatica.models.repositories;

import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.Fuente;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.FuenteEstatica;
import java.util.List;
import java.util.Optional;

public interface IFuenteEstaticaRepository {

  FuenteEstatica save(FuenteEstatica fuenteEstatica);
  List<FuenteEstatica> findAll();
  Fuente findById(Long id); //Optional puede devolver una fuente o null (si no la encuentra)


  void delete(FuenteEstatica fuenteEstatica);
}
